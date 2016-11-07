#!/usr/bin/env python3

import rsa
import argparse


def main():
    parser = argparse.ArgumentParser(description='Crack a public key')
    parser.add_argument('-e', dest='e', action='store', default=-1,
                        help='encryption key')
    parser.add_argument('-c', dest='c', action='store', default=-1,
                        help='common key')
    args = parser.parse_args()
    e = str(args.e)
    c = int(args.c)

    if e == -1 or c == -1:
        parser.print_help()
        return

    print("Factoring c...")
    factors = sorted(list(rsa.factors(c) - {c, 1}))
    a = factors[-1]
    b = factors[-2]

    print("c's factors are {} and {}".format(a, b))

    m = (a - 1) * (b - 1)
    pub = rsa.public_key(m, c)
    priv = rsa.private_key(pub[0], m, c)

    print("Public Key: {}\nPrivate Key: {}".format(pub, priv))
    print("Of course that all depends on where you start counting e at")

if __name__ == "__main__":
    main()
