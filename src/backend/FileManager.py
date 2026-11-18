class FileManager:
    def __init__(self, filePath):
        self.filePath = filePath

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

    def write_file(self):
        pass