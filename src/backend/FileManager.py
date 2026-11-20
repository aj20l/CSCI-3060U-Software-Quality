class FileManager:
    def __init__(self, filePath):
        self.filePath = filePath

    # reads from a given file, entering each line into a list to be returned
    def read_file(self, file):
        entry = []
        with open(self.filePath + "/" + file, "r", encoding="utf-8") as f:
            while True:
                line = f.readline().strip()
                if line == "END" or line == "":
                    break
                entry.append(line)
        f.close()
        return entry

    # writes to a given file from a list of data
    def write_file(self, file, data_list):
        with open(self.filePath + "/" + file, "w", encoding="utf-8") as f:
            for data in data_list:
                f.write(str(data) + "\n")
            f.write("END\n")
