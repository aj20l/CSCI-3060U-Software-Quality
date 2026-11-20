import backend.Codes as Code

class GameCollection:
    def __init__(self, game, username):
        self.game = game
        self.username = username

    def __str__(self):
        return f"{self.game.ljust(Code.GAME_NAME_WIDTH, " ")} {self.username.ljust(Code.USERNAME_WIDTH, " ")}"

class GameCollectionManager:
    def __init__(self):
        self.game_collection = []

    def load_collection(self, data):
        game = data[:27]
        username = data [27:36]
        self.game_collection.append(GameCollection(game, username))

    def list_collection(self):
        for collection in self.game_collection:
            print(collection)




