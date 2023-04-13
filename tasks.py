#!/usr/bin/env python3

from invoke import task

@task
def build(c):
    c.run("gcc -o main main.c -Wall -Werror -Wpedantic")