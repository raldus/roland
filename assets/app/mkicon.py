#!/usr/bin/env python3
print("static const unsigned char IMAGE_DATA[] = {{{}}};".format(
",".join(str(b) for b in open("icon32x32-256.bmp", "rb").read())))
