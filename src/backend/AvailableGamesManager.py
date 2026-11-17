
class AvailableGamesManager:
    gamesList=[]

    def loadGames(self):
        #load from available games file
        file = open("dailyTransaction.txt", "r")
        for line in file:
            newGame=Game(line)
            self.gamesList.append(newGame)
        file.close()


    def saveGames(self):
        #write to file
        pass












class Game:
    name=""
    sellername=""
    price=0
    def __init__(self, line):
        self.name,self.sellername,self.price=line.split("_")