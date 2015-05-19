# -*- coding: utf-8 -*-

# Copyright (c) 2015, Shinya Yagyu
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 3. Neither the name of the copyright holder nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.


import unittest
import chizer


class TestChizer(unittest.TestCase):

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_is_file_encrypted(self):
        self.assertTrue(chizer.is_file_encrypted("data/faked.dat"),
                        "file encryption check test for faked data")
        self.assertFalse(chizer.is_file_encrypted(
            "data/Storj - Decentralizing Cloud Storage-vl3bUzfn2lg.mp4.gz"),
            "file encryption check test for gzipped move file")

    def test_are_chunks_encrypted(self):
        self.assertFalse(chizer.are_chunks_encrypted("data/faked.dat"),
                         "chunks encryption check test for faked data")
        self.assertTrue(chizer.are_chunks_encrypted(
            "data/ac59ab5a282afd3de22062c7d62b5367"),
            "chuns encryption check test for encrypted file")

if __name__ == '__main__':
    unittest.main()
