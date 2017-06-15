#!/usr/bin/python

import os
import sys
import argparse
import copy
import filecmp
import glob
import subprocess
import shutil
from distutils.spawn import find_executable

#==============================================================================#

def ShouldModify(file1, file2):
    return not filecmp.cmp(file1, file2, shallow=False)

#==============================================================================#

def ComposeFilesToModify(data_dir, type):
    files = []
    if(type == "safe" or type == "all"):
        files.append(os.path.join(data_dir, "hosts.safe"))
    if(type == "ultrasafe" or type == "all"):
        files.append(os.path.join(data_dir, "hosts.ultrasafe"))
    return files

#==============================================================================#

def ComposeDataDir(prefix, data_folder):
    if(os.path.commonprefix([prefix, data_folder])):
        data_folder.replace(os.path.commonprefix([prefix, data_folder]), "")
    return os.path.join(prefix, data_folder)

#==============================================================================#

def install_cmd(args):
    exe_install_path = os.path.join(args.prefix, "bin/ModifyTrackingBlocker")

    if(not run_command.wrap_to_run_as_root(exe_install_path, sys.argv[0])):
        os.symlink(os.path.realpath(sys.argv[0]), exe_install_path)

    data_dir = ComposeDataDir(args.prefix, args.data_folder)
    for f in args.files:
        shutil.copy2(f, os.path.join(data_dir, os.path.basename(f)))

#==============================================================================#

def lc(file):
    return sum(1 for l in open(file))

#==============================================================================#

def display(args, data_dir):
    lc1 = lc(os.path.join(args.host_folder, "hosts"))
    lc2 = lc(os.path.join(data_dir, "hosts.original"))
    print os.path.join(args.host_folder, "hosts"),"is blocking approximately",lc1-lc2,"domains"

#==============================================================================#

def status_cmd(args):
    data_dir = ComposeDataDir(args.prefix, args.data_folder)

    if(args.command == "original" or args.command == "off"):
        f1 = os.path.join(data_dir, "hosts.original")
        f2 = os.path.join(args.host_folder, "hosts")
        if(ShouldModify(f1, f2)):
            shutil.copy2(f1, f2)
    elif(args.command == "safe"):
        f1 = os.path.join(data_dir, "hosts.safe")
        f2 = os.path.join(args.host_folder, "hosts")
        if(ShouldModify(f1, f2)):
            shutil.copy2(f1, f2)
    elif(args.command == "ultrasafe" or args.command == "on"):
        f1 = os.path.join(data_dir, "hosts.ultrasafe")
        f2 = os.path.join(args.host_folder, "hosts")
        if(ShouldModify(f1, f2)):
            shutil.copy2(f1, f2)
    elif(args.command == "display"):
        display(args, data_dir)

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

    os.environ["PATH"] += ":" + args.rm_ctrl_M
    rm_ctrl_M_path = find_executable("rm_CTRL-M")
    if(not rm_ctrl_M_path):
        home_folders = ["/home", "/Users"]
        for home in home_folders:
            for root, dirs, files in os.walk(home):
                for dirhome in dirs:
                    for hroot, hdirs, hfiles in os.walk(os.path.join(root,dirhome)):
                        lvls=len(hroot.split("/"))
                        if(lvls > 4):
                            break
                        for hdir in hdirs:
                            print 'checking',os.path.join(hroot,hdir)
                            if "Scripts" in hdir:
                                os.environ["PATH"] += ":" + os.path.join(hroot,hdir)
                                break
        rm_ctrl_M_path = find_executable("rm_CTRL-M")
    print os.environ["PATH"]
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
                        default="share/TrackingBlocker", nargs=1)
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
                             choices=['original', 'off', 'on', 'safe',
                                      'ultrasafe', 'display'])
    stat_choice.set_defaults(func=status_cmd)

    # Add option
    add_choice = subparser.add_parser("add", help="Add domain to TrackingBlocker")
    add_choice.add_argument("-t", "--type", help="Tracking type to add to",
                            choices=['all','safe','ultrasafe'],
                            default="all")
    add_choice.add_argument("domain", nargs='*', help="Domains to add")
    add_choice.set_defaults(func=add_cmd)

    # Remove option
    remove_choice = subparser.add_parser("remove", help="Remove domain from "
        + "TrackingBlocker (removed domains are added to "
        + "{PREFIX}/share/TrackingBlocker/hosts.{type}.removed)")
    remove_choice.add_argument("-t", "--type", help="Tracking type to remove from",
                               choices=['all','safe','ultrasafe'],
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
                               choices=['all','safe','ultrasafe'],
                               default="ultrasafe")
    update_choice.add_argument("--exe-path", help="Path to combine_hosts exe",
                               default="../libexec")
    update_choice.add_argument("--exe-name", help="Alternative name to \"combine_hosts\"",
                               default="combine_hosts")
    update_choice.add_argument("--rm-ctrl-M", help="Script to remove ^M characters",
                               default="/home/jmadsen/Scripts")
    update_choice.set_defaults(func=update_cmd)

    #if(not subprocess.call(join(["sudo", "-n", "echo", "-n"], ' '))):
    #    print sys.argv[0],"must be executed as superuser"
    #    sys.exit(1)

    args = parser.parse_args()
    data_dir = ComposeDataDir(args.prefix, args.data_folder)
    #print args,";","data_dir = ",data_dir
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
    import run_command
    main(sys.argv[1:])

#==============================================================================#
