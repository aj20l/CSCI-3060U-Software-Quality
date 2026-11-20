import Codes as Code

class Game:
    def __init__(self, game, seller, price):
        self.game = game
        self.seller = seller
        self.price = price

    def __str__(self):
        return f"{self.game.ljust(Code.GAME_NAME_WIDTH, " ")}{self.seller.ljust(Code.SELLER_USERNAME_WIDTH, " ")}{self.price.ljust(Code.PRICE_WIDTH, " ")}"

class AvailableGamesManager:
    def __init__(self):
        self.available_games = []

    def load_game(self, data):
        game = data[:27]
        seller = data[27:43]
        price = data [43:50]
        self.available_games.append(Game(game, seller, price))

    def get_games(self):
        return self.available_games

    def list_games(self):
        for game in self.available_games:
            print(game)









