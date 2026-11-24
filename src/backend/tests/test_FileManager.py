import pytest

from FileManager import FileManager
import Codes as Code

class TestFileManager:
    def test__init__(self):
        test_directory = "data"
        manager = FileManager(test_directory)
        assert manager.filePath == test_directory

    def test_read_file(self):
        file = "currentaccounts.txt"
        test_directory = "tests/data"
        expected = [
            "admin01         AA 000500.00",
            "fulluser        FS 000100.00",
            "buyer01         BS 000100.00",
            "seller01        SS 000050.00",
            "pooruser        BS 000010.00",
            "deleteme        FS 000020.00",
            "sysadmin        AA 000900.00"
        ]

        manager = FileManager(test_directory)
        line = manager.read_file(file)

        assert line == expected

    def test_write_file(self):
        file = "currentaccounts.txt"
        test_directory = "tests/data"
        data = [
            "admin01         AA 000500.00",
            "fulluser        FS 000100.00",
            "buyer01         BS 000100.00",
            "seller01        SS 000050.00",
            "pooruser        BS 000010.00",
            "deleteme        FS 000020.00",
            "sysadmin        AA 000900.00",
            "newplayer       FS 000000.00"
        ]

        manager = FileManager(test_directory)
        manager.write_file(file, data)

        result = manager.read_file(file)

        assert result == data

        # undos the write from the test
        manager.write_file(file, [
            "admin01         AA 000500.00",
            "fulluser        FS 000100.00",
            "buyer01         BS 000100.00",
            "seller01        SS 000050.00",
            "pooruser        BS 000010.00",
            "deleteme        FS 000020.00",
            "sysadmin        AA 000900.00",
        ])
