import Codes as Code

# help class for game information
class Game:
    def __init__(self, name, seller, price):
        self.name = name
        self.seller = seller
        self.price = price

    def __str__(self):
        return f"{self.name.ljust(Code.GAME_NAME_WIDTH, " ")} {self.seller.ljust(Code.SELLER_USERNAME_WIDTH, " ")} {self.price.ljust(Code.PRICE_WIDTH, " ")}"

# manager class for available game related data processing
class AvailableGamesManager:
    def __init__(self):
        self.available_games = []

    # slices a transactions input data to obtain needed data to load an available game
    def load_game(self, data):
        offset = 0
        name = data[offset:offset + Code.GAME_NAME_WIDTH]
        offset += Code.GAME_NAME_WIDTH + 1
        seller = data[offset:offset + Code.USERNAME_WIDTH]
        offset += Code.USERNAME_WIDTH + 1
        price = data [offset:offset + Code.PRICE_WIDTH]

        self.available_games.append(Game(name, seller, price))

    # returns the list of available games
    def get_games(self):
        return self.available_games