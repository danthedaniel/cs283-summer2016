#!/usr/bin/env python3

import rsa
import argparse


def main():
    parser = argparse.ArgumentParser(description='Connect to RSA encrypted chat server')
    parser.add_argument('-mprime', dest='m', action='store', default=-1,
                        help='Mth prime to select')
    parser.add_argument('-nprime', dest='n', action='store', default=-1,
                        help='Nth prime to select')
    args = parser.parse_args()
    m = int(args.m)
    n = int(args.n)

    if m == -1 or n == -1:
        parser.print_help()
        return

    keys = rsa.generate_keys(m=m, n=n)

    print("Public Key: {}\nPrivate Key: {}\n".format(keys['public'], keys['private']))


if __name__ == "__main__":
    main()
