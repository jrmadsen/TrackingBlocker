#!/usr/bin/python2

import os
import sys
import argparse
import filecmp
import shutil
from distutils.spawn import find_executable

#==============================================================================#

def ShouldModify(file1, file2):
    return not filecmp.cmp(file1, file2, shallow=False)

#==============================================================================#

def ComposeFilesToModify(data_dir, type):
    files = []
    if(type == "0" or type == "all"):
        files.append(os.path.join(data_dir, "hosts.profile.0"))
    return files

#==============================================================================#

def ComposeDataDir(prefix, data_folder):
    if(os.path.commonprefix([prefix, data_folder])):
        data_folder.replace(os.path.commonprefix([prefix, data_folder]), "")
    return os.path.join(prefix, data_folder)

#==============================================================================#

def install_cmd(args):
    exe_install_path = os.path.join(args.prefix, "bin/tracking_blocker")

    if(not run_command.wrap_to_run_as_root(exe_install_path, sys.argv[0])):
        os.symlink(os.path.realpath(sys.argv[0]), exe_install_path)

    data_dir = ComposeDataDir(args.prefix, args.data_folder)
    for f in args.files:
        shutil.copy2(f, os.path.join(data_dir, os.path.basename(f)))

#==============================================================================#

def get_unique(_l):
    _s = set()
    for i in _l:
        _s.add(i)
    return list(_s)

#==============================================================================#
# line count function
def lc(file):
    #return sum(1 for l in open(file))
    f = open(file, 'r')
    lines = f.readlines()
    f.close()
    # pop out domains from lines
    prefixes = ["127.0.0.1"," ","www."]
    index_delete = []
    # loop over lines
    for i in range(0, len(lines)):
        # strip newline and tab
        lines[i] = lines[i].strip("\n")
        lines[i] = lines[i].strip("\t")
        # skip comments and empty lines
        if not lines[i] or (len(lines[i]) > 0 and lines[i][0] == '#'):
            index_delete.append(i)
            continue
        # strip prefixes
        for p in prefixes:
            lines[i] = lines[i].strip(p)
    # remove empty lines and comments
    for i in reversed(index_delete):
        lines.pop(i)
    # alphabetically sort
    lines = sorted(lines, key=str.lower)
    # get unique entries
    lines = get_unique(lines)

    # return length of lines
    return len(lines)

#==============================================================================#
# display number of domains blocked
def display(args, data_dir):
    lc1 = lc(os.path.join(args.host_folder, "hosts"))
    lc2 = lc(os.path.join(data_dir, "hosts.original"))
    print os.path.join(args.host_folder, "hosts"), "is blocking approximately", \
        lc1-lc2, "domains (www-prefixed and non-www-prefixed e.g., www.ad-site.com and ad-site.com)"

#==============================================================================#
# handle status commands
def status_cmd(args):
    data_dir = ComposeDataDir(args.prefix, args.data_folder)

    if(args.command == "original" or args.command == "off"):
        f1 = os.path.join(data_dir, "hosts.original")
        f2 = os.path.join(args.host_folder, "hosts")
        if(ShouldModify(f1, f2)):
            shutil.copy2(f1, f2)
    elif(args.command == "0" or args.command == "on"):
        f1 = os.path.join(data_dir, "hosts.profile.0")
        f2 = os.path.join(args.host_folder, "hosts")
        if(ShouldModify(f1, f2)):
            shutil.copy2(f1, f2)
    elif(args.command == "display"):
        display(args, data_dir)
    else:
        f1 = os.path.join(data_dir, "hosts.profile.%i" % (int(args.command)))
        f2 = os.path.join(args.host_folder, "hosts")
        if os.path.exists(f1):
            if(ShouldModify(f1, f2)):
                shutil.copy2(f1, f2)

    if(args.command != "display"):
        display(args, data_dir)

#==============================================================================#

def add_cmd(args):
    data_dir = ComposeDataDir(args.prefix, args.data_folder)
    files = ComposeFilesToModify(data_dir, args.type)

    for fname in files:
        print 'Adding domains to',fname
        f = open(fname, 'a')
        for d in args.domain:
            d = d.lstrip("www.")
            f.write("127.0.0.1  " + d + "\n")
            f.write("127.0.0.1  www." + d + "\n")
        f.close()

#==============================================================================#

def remove_cmd(args):
    data_dir = ComposeDataDir(args.prefix, args.data_folder)
    files = ComposeFilesToModify(data_dir, args.type)
    for fname in files:
        print 'Removing domains from',fname
        # remove file
        rfname = os.path.join(data_dir, os.path.basename(fname) + ".removed")
        # load previous removes
        if(args.load_previous_removes):
            if(os.path.exists(rfname)):
                fr = open(rfname, 'r')
                rlines = fr.readlines()
                fr.close()
                for line in rlines:
                    line = line.rstrip('\n')
                    args.domain.append(line)
            else:
                continue
        # read current file
        f = open(fname, 'r')
        lines = f.readlines()
        f.close()
        # pop out domains from lines
        subtract = 0
        prefixes = ["","www."]
        for i in range(0, len(lines)):
            for d in args.domain:
                for p in prefixes:
                    dstr = "127.0.0.1  " + p + d
                    if(not args.all_matching and not args.load_previous_removes):
                        dstr += " "
                    if(dstr in lines[i - subtract]):
                        line2 = lines[i - subtract].strip("127.0.0.1")
                        lines.pop(i - subtract)
                        subtract += 1
                        line2 = line2.strip('\n')
                        line2 = line2.strip()
                        print 'Removing',line2,"..."
                        if(not args.load_previous_removes and not p):
                            fr = open(rfname, 'a')
                            fr.write(line2 + '\n')
                            fr.close()
        f = open(fname, 'w')
        for line in lines:
            f.write(line)
        f.close()

#==============================================================================#

def update_cmd(args):

    rm_ctrl_M_path = os.path.normpath(os.path.abspath(
        os.path.join(sys.argv[0],
                     os.path.join(args.rm_ctrl_M,
                                  "remove-ctrl-M.bash"))))

    if(not os.path.exists(rm_ctrl_M_path)):
        rm_ctrl_M_path = find_executable(os.path.join(args.rm_ctrl_M,
                                                      "remove-ctrl-M.bash"))

        if(not os.path.exists(rm_ctrl_M_path)):
            rm_ctrl_M_path = find_executable("remove-ctrl-M.bash")

            if(not os.path.exists(rm_ctrl_M_path)):
                print "Unable to find \"remove-ctrl-M.bash\""
                sys.exit(1)

    combine_hosts_path = os.path.normpath(os.path.abspath(
        os.path.join(sys.argv[0],
                     os.path.join(args.exe_path,
                                  args.exe_name))))

    if(not os.path.exists(combine_hosts_path)):
        combine_hosts_path = find_executable(os.path.join(args.exe_path,
                                                          args.exe_name))
        if(not os.path.exists(combine_hosts_path)):
            combine_hosts_path = find_executable(args.exe_name)

            if(not os.path.exists(combine_hosts_path)):
                print 'Unable to find',args.exe_name
                sys.exit(1)

    data_dir = ComposeDataDir(args.prefix, args.data_folder)
    output_files = ComposeFilesToModify(args.output_dir, args.type)
    for ofile in output_files:
        cmd = []
        cmd.append(combine_hosts_path)
        cmd.extend(["-U", os.path.abspath(args.output_dir)])
        if(args.update):
            cmd.append("-u")
        cmd.extend(["-o", os.path.basename(ofile)])
        cmd.extend(["-d", os.path.abspath(args.default)])
        if(len(args.files) > 0):
            cmd.append("-f")
            for f in args.files:
                cmd.append(os.path.abspath(f))
        ret = run_command.run_command(cmd)
        if(ret > 0):
            print 'Error running command:', join(cmd, ' ')
            sys.exit(ret)
        else:
            shutil.copy2(ofile, os.path.join(data_dir, os.path.basename(ofile)))

    cmd = []
    cmd.extend([sys.argv[0], "remove", "-t", args.type, "-l"])
    run_command.run_command(cmd)

#==============================================================================#

def main(argv):
    parser = argparse.ArgumentParser()

    # Common options
    parser.add_argument("--prefix", help="Prefix for install data",
                        default="/usr/local", nargs=1)
    parser.add_argument("-d", "--data-folder",
                        help="Data folder containing host files - {PREFIX}/...",
                        default="etc/TrackingBlocker", nargs=1)
    parser.add_argument("--host-folder", help="Folder where host files reside",
                        default="/etc", nargs=1)
    subparser = parser.add_subparsers(help="sub-command help")

    # Install option
    install_choice = subparser.add_parser("install", help="Install TrackingBlocker")
    install_choice.add_argument("files", nargs='*', help="Host files to install")
    install_choice.set_defaults(func=install_cmd)

    # Status option
    stat_choice = subparser.add_parser("status", help="TrackingBlocker status change/display")
    stat_choice.add_argument("command", help="Parser command",
                             choices=['original', 'off', 'on',
                                      'profile.0', 'display'])
    stat_choice.set_defaults(func=status_cmd)

    # Add option
    add_choice = subparser.add_parser("add", help="Add domain to TrackingBlocker")
    add_choice.add_argument("-t", "--type", help="Tracking type to add to",
                            choices=['all','profile.0'],
                            default="all")
    add_choice.add_argument("domain", nargs='*', help="Domains to add")
    add_choice.set_defaults(func=add_cmd)

    # Remove option
    remove_choice = subparser.add_parser("remove", help="Remove domain from "
        + "TrackingBlocker (removed domains are added to "
        + "{PREFIX}/etc/TrackingBlocker/hosts.{type}.removed)")
    remove_choice.add_argument("-t", "--type", help="Tracking type to remove from",
                               choices=['all','profile.0'],
                               default="all")
    remove_choice.add_argument("-a", "--all-matching", action="store_true",
                               help="Remove all domains starting with domains listed")
    remove_choice.add_argument("-l", "--load-previous-removes", action="store_true",
                               help="Reremove previous stored in hosts.{type}.removed")
    remove_choice.add_argument("domain", nargs='*', help="Domains to remove")
    remove_choice.set_defaults(func=remove_cmd)

    update_choice = subparser.add_parser("update", help="Update domains")
    update_choice.add_argument("-f", "--files", help="Files to include in update",
                               nargs='*')
    update_choice.add_argument("-u", "--update", help="Update with WINHOSTS",
                               action="store_true")
    update_choice.add_argument("-o", "--output-dir", help="temporary folder to hold outputs",
                               default="/tmp/4wejsdfasdff")
    update_choice.add_argument("-d","--default", help="default HOST file",
                               required=True)
    update_choice.add_argument("-t", "--type", help="Tracking type to update",
                               choices=['all','profile.0'],
                               default="profile.0")
    update_choice.add_argument("--exe-path", help="Path to combine_hosts exe",
                               default="../libexec")
    update_choice.add_argument("--exe-name", help="Alternative name to \"combine_hosts\"",
                               default="combine_hosts")
    update_choice.add_argument("--rm-ctrl-M", help="Script to remove ^M characters",
                               default="pyscripts")
    update_choice.set_defaults(func=update_cmd)

    #if(not subprocess.call(join(["sudo", "-n", "echo", "-n"], ' '))):
    #    print sys.argv[0],"must be executed as superuser"
    #    sys.exit(1)

    args = parser.parse_args()
    data_dir = ComposeDataDir(args.prefix, args.data_folder)
    if(not os.path.exists(data_dir) and args.func != install_cmd):
        print 'TrackingBlocker has not been installed'
        sys.exit(1)
    elif(not os.path.exists(data_dir)):
        os.makedirs(data_dir)
    args.func(args)



#==============================================================================#

if __name__ == "__main__":
    script_path = os.path.abspath(os.path.dirname(os.path.realpath(sys.argv[0])))
    sys.path.append(os.path.join(script_path, "pypath"))
    from run_command import run_command, join
    main(sys.argv[1:])

#==============================================================================#
