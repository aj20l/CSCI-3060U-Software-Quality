import pytest

from AvailableGamesManager import AvailableGamesManager, Game
import Codes as Code

class TestGame:
    def test__init__(self):
        name = "Spiderman                 "
        seller = "seller01       "
        price = "015.00"

        game = Game(name, seller, price)

        assert game.name == name
        assert game.seller == seller
        assert game.price == price

    def test__str__(self):
        name = "Spiderman                 "
        seller = "seller01       "
        price = "015.00"
        expected = name + " " + seller + " " + price

        game = Game(name, seller, price)

        assert str(game) == expected


class TestAvailableGamesManager:
    def test__init__(self):
        manager = AvailableGamesManager()
        assert manager.available_games == []


    def test_load_game(self):
        name = "Spiderman                 "
        seller = "seller01       "
        price = "015.00"
        data = name + " " + seller + " " + price

        manager = AvailableGamesManager()
        manager.load_game(data)

        assert len(manager.available_games) == 1
        assert manager.available_games[0].name == name
        assert manager.available_games[0].seller == seller
        assert manager.available_games[0].price == price


    def test_get_games(self):
        name_one = "Spiderman                 "
        seller_one = "seller01       "
        price_one = "015.00"
        name_two = "Hades                     "
        seller_two = "seller01       "
        price_two = "024.99"

        manager = AvailableGamesManager()

        game_one = Game(name_one, seller_one, price_one)
        game_two = Game(name_two, seller_two, price_two)

        manager.available_games.append(game_one)
        manager.available_games.append(game_two)

        assert manager.get_games() == [game_one, game_two]
