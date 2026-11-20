import Codes as Code

# help class for game information
class Game:
    def __init__(self, game, seller, price):
        self.game = game
        self.seller = seller
        self.price = price

    def __str__(self):
        return f"{self.game.ljust(Code.GAME_NAME_WIDTH, " ")}{self.seller.ljust(Code.SELLER_USERNAME_WIDTH, " ")}{self.price.ljust(Code.PRICE_WIDTH, " ")}"

# manager class for available game related data processing
class AvailableGamesManager:
    def __init__(self):
        self.available_games = []

    # splices a transactions input data to obtain needed data to load an available game
    def load_game(self, data):
        game = data[:27]
        seller = data[27:43]
        price = data [43:50]
        self.available_games.append(Game(game, seller, price))

    # returns the list of available games
    def get_games(self):
        return self.available_games

    def list_games(self):
        for game in self.available_games:
            print(game)









