import pytest

from GameCollectionManager import GameCollectionManager, GameCollection
import Codes as Code

class TestGameCollection:
    def test__init__(self):
        name = "Celeste                   "
        username = "fulluser       "

        game_collection = GameCollection(name, username)

        assert game_collection.name == name
        assert game_collection.username == username

    def test__str__(self):
        name = "Celeste                   "
        username = "fulluser       "
        expected = name + " " + username

        game_collection = GameCollection(name, username)

        assert str(game_collection) == expected


class TestGameCollectionManager:
    def test__init__(self):
        manager = GameCollectionManager()
        assert manager.game_collection == []

    def test_load_collection(self):
        name = "Celeste                   "
        username = "fulluser       "
        data = name + " " + username

        manager = GameCollectionManager()
        manager.load_collection(data)

        result = manager.game_collection

        assert len(result) == 1
        assert result[0].name == name
        assert result[0].username == username

    def test_get_collestions(self):
        game_one = "Celeste                   "
        username_one = "fulluser       "
        game_two = "GTA V                     "
        username_two = "admin01        "

        game_collection_one = GameCollection(game_one, username_one)
        game_collection_two = GameCollection(game_two, username_two)

        manager = GameCollectionManager()
        manager.game_collection.append(game_collection_one)
        manager.game_collection.append(game_collection_two)

        assert manager.game_collection == [game_collection_one, game_collection_two]