#!/usr/bin/env python

import os
import sys
import subprocess
from distutils.spawn import find_executable

#==============================================================================#
# join a list
def join(cmd, delim):
    return delim.join(cmd)

#==============================================================================#

def wrap_to_run_as_root(exe_install_path, true_command, expand_path = True):
    run_as_root_path = find_executable("run-as-root-wrapper")

    if(not run_as_root_path):
        return False
    else:
        if(os.path.exists(exe_install_path)):
            os.unlink(exe_install_path)

        if(expand_path):
            true_command = os.path.realpath(true_command)
            true_command = os.path.abspath(true_command)
            true_command = os.path.normpath(true_command)

        f = open(exe_install_path, 'w')
        f.write("#!/bin/bash\n\n")
        f.write(run_as_root_path + " " + true_command + " $@\n\n")
        f.close()
        os.chmod(exe_install_path, 755)

        return True

#==============================================================================#
def wrap_to_run_as_root_w_args(exe_install_path, true_command, expand_path = True):
    run_as_root_path = find_executable("run-as-root-wrapper")

    if(not run_as_root_path or not len(exe_install_path) == len(true_command)):
        return False
    else:
        for i in range(len(exe_install_path)):

            if(expand_path):
                true_command[i][0] = os.path.realpath(true_command[i][0])
                true_command[i][0] = os.path.abspath(true_command[i][0])
                true_command[i][0] = os.path.normpath(true_command[i][0])

            if(os.path.exists(exe_install_path[i])):
                os.unlink(exe_install_path[i])

            cmd = join(true_command[i], ' ')

            f = open(exe_install_path[i], 'w')
            f.write("#!/bin/bash\n\n")
            f.write(run_as_root_path + " " + cmd + " $@\n\n")
            f.close()
            os.chmod(exe_install_path[i], 755)

        return True

#==============================================================================#
# run a command with no input or output
def run(cmd):
    return subprocess.call(cmd, shell=True)

#==============================================================================#
# run a command with an input
def run_in(cmd, infile):
    inf = open(infile, 'rb', 0)
    process = subprocess.Popen(cmd, shell=True, stdin=inf)
    rflag = process.wait()
    return rflag

#==============================================================================#
# run a command with an output
def run_out(cmd, outFile, errFile, appendOut, appendErr):
    outMode = 'w'
    errMode = 'w'
    if(appendOut):
        outMode = 'a'
    if(appendErr):
        errMode = 'a'

    rflag = 0
    if(outFile and errFile):
        out = open(outFile, outMode)
        err = open(errFile, errMode)
        process = subprocess.Popen(cmd, shell=True, stdout=out, stderr=err)
        rflag = process.wait()
    elif(outFile and not errFile):
        out = open(outFile, outMode)
        process = subprocess.Popen(cmd, shell=True, stdout=out, stderr=None)
        rflag = process.wait()
    elif(errFile and not outFile):
        err = open(errFile, errMode)
        process = subprocess.Popen(cmd, shell=True, stdout=None, stderr=err)
        rflag = process.wait()
    else:
        process = subprocess.Popen(cmd, shell=True, stdout=None, stderr=None)
        rflag = process.wait()

    return rflag

#==============================================================================#
# run a command with an input and an output
def run_in_out(cmd, inFile, outFile, errFile, appendOut, appendErr):
    outMode = 'w'
    errMode = 'w'
    if(appendOut):
        outMode = 'a'
    if(appendErr):
        errMode = 'a'
    inf = open(inFile, 'rb', 0)

    rflag = 0
    if(outFile and errFile):
        out = open(outFile, outMode)
        err = open(errFile, errMode)
        process = subprocess.Popen(cmd, shell=True, stdin=inf, stdout=out, stderr=err)
        rflag = process.wait()
    elif(outFile and not errFile):
        out = open(outFile, outMode)
        process = subprocess.Popen(cmd, shell=True, stdin=inf, stdout=out, stderr=None)
        rflag = process.wait()
    elif(errFile and not outFile):
        err = open(errFile, errMode)
        process = subprocess.Popen(cmd, shell=True, stdin=inf, stdout=None, stderr=err)
        rflag = process.wait()
    else:
        process = subprocess.Popen(cmd, shell=True, stdin=inf, stdout=None, stderr=None)
        rflag = process.wait()

    return rflag

#==============================================================================#
# determine which run command needs to be called
def run_command(cmd):
    new_cmd = []
    inFile = ""
    outFile = ""
    errFile = ""
    appendOut = False
    appendErr = False
    skip = False
    for i in range(len(cmd)):
        if(cmd[i] == "<"):
            inFile = cmd[i+1]
            skip = True
        elif(cmd[i] == ">"):
            outFile = cmd[i+1]
            skip = True
        elif(cmd[i] == ">&"):
            outFile = cmd[i+1]
            errFile = cmd[i+1]
            skip = True
        elif(cmd[i] == "1>"):
            outFile = cmd[i+1]
            skip = True
        elif(cmd[i] == "2>"):
            errFile = cmd[i+1]
            skip = True
        elif(cmd[i] == "1>>"):
            outFile = cmd[i+1]
            appendOut = True
            skip = True
        elif(cmd[i] == "2>>"):
            errFile = cmd[i+1]
            appendErr = True
            skip = True
        else:
            if(not skip):
                new_cmd.append(cmd[i])
            else:
                skip = False

    new_cmd = join(new_cmd, " ")

    if(inFile and outFile):
        return run_in_out(new_cmd, inFile, outFile, errFile, appendOut, appendErr)
    elif(inFile):
        return run_in(new_cmd, inFile)
    elif(outFile):
        return run_out(new_cmd, outFile, errFile, appendOut, appendErr)
    else:
        return run(new_cmd)

#==============================================================================#

def run_as_root(command, args, expand_path = True):
    run_as_root_path = find_executable("run-as-root-wrapper")

    if(not run_as_root_path):
        return 1
    else:
        if(expand_path):
            command = os.path.realpath(command)
            command = os.path.abspath(command)
            command = os.path.normpath(command)

        cmd = []
        cmd.append(run_as_root_path)
        cmd.append(command)
        cmd.extend(args)

        return subprocess.call(' '.join(cmd), shell=True)
