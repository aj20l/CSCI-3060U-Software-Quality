class GameCollectionManager:
    gameCollections={"",[]}
    def loadGameCollections(self):
        #file reading

        file = open("gameCollection.txt", "r")
        for line in file:
            name,ownerName=line.split("_")

            self.gameCollections[ownerName].append(name)
        file.close()

    def saveGameCollections(self):
        pass




