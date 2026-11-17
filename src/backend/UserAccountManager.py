import GameCollectionManager
class UserAccountsManager:
    userAccountsList=[]

    def loadUsers(self):
        #return list/array
        file = open("dailyTransaction.txt", "r")
        for line in file:
            newUser=User(line)
            self.userAccountsList.append(newUser)
        file.close()
        pass
    def saveUsers(self):
        pass



class User:
    username=""
    userType=""
    credit=0
    games=[]
    def __init__(self, line):
        self.username,self.userType,self.credit=line.split("_")
    def addGame(self,games):
        for game in games[self.username]:
            self.games.append(game)
            #games collection dict structure user has an array of games



