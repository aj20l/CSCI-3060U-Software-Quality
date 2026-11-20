import Codes as Code

# helper class for game collection information
class GameCollection:
    def __init__(self, game, username):
        self.game = game
        self.username = username

    def __str__(self):
        return f"{self.game.ljust(Code.GAME_NAME_WIDTH, " ")}{self.username.ljust(Code.USERNAME_WIDTH, " ")}"

# manager class for game collection related data processing
class GameCollectionManager:
    def __init__(self):
        self.game_collection = []

    # splices a transactions input data to obtain needed data to load game collection 
    def load_collection(self, data):
        game = data[:27]
        username = data [27:36]
        self.game_collection.append(GameCollection(game, username))

    # returns the list of games collections
    def get_collestions(self):
        return self.game_collection

    def list_collection(self):
        for collection in self.game_collection:
            print(collection)




