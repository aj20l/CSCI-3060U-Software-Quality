# Change these to be potentially global variables
GAME_NAME_WIDTH = 26
SELLER_USERNAME_WIDTH = 15
PRICE_WIDTH = 6

class Game:
    def __init__(self, game, seller, price):
        self.game = game
        self.seller = seller
        self.price = price

    def __str__(self):
        return f"{self.game.ljust(GAME_NAME_WIDTH, " ")} {self.seller.ljust(SELLER_USERNAME_WIDTH, " ")} {self.price.ljust(PRICE_WIDTH, " ")}"

class AvailableGamesManager:
    def __init__(self):
        self.available_games = []

    def load_game(self, data):
        game = data[:27]
        seller = data[27:36]
        price = data [36:50]
        self.available_games.append(Game(game, seller, price))

    def saveUsers(self):
        pass

    def list_games(self):
        for game in self.available_games:
            print(game)









