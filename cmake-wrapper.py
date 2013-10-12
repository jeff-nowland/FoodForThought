#!/usr/bin/python3

from argparse import ArgumentParser

parser = ArgumentParser(description="Small cmake wrapper")

args, cmakeargs = parser.parse_known_args()

from subprocess import list2cmdline, check_call, CalledProcessError

cmdargs=[
    f'cmake {list2cmdline(cmakeargs)}'
]
cmd=' && '.join(cmdargs)

print(f'Running {cmd!r}')

from sys import exit

try:
    check_call(cmd, shell=True)
except CalledProcessError as err:
    exit(err.returncode)
