import Codes as Code

# helper class for game collection information
class GameCollection:
    def __init__(self, name, username):
        self.name = name
        self.username = username

    def __str__(self):
        return f"{self.name.ljust(Code.GAME_NAME_WIDTH, " ")} {self.username.ljust(Code.USERNAME_WIDTH, " ")}"

# manager class for game collection related data processing
class GameCollectionManager:
    def __init__(self):
        self.game_collection = []

    # slices a transactions input data to obtain needed data to load game collection 
    def load_collection(self, data):
        offset = 0
        name = data[offset:offset + Code.GAME_NAME_WIDTH]
        offset += Code.GAME_NAME_WIDTH + 1
        username = data [offset:offset + Code.USERNAME_WIDTH]

        self.game_collection.append(GameCollection(name, username))

    # returns the list of games collections
    def get_collestions(self):
        return self.game_collection