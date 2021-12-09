library(stringr)

olympics <- read.csv("athlete_events.csv")

olympics$Sport <- paste(olympics$Sport, olympics$Year, sep = " ")
olympics$Medal <- paste("(", olympics$Medal, ")")
olympics$Event <- paste(olympics$Event, olympics$Medal, sep = " ")

olympics <- olympics[, -c(1, 3, 4, 5, 6, 8, 9, 10, 11, 12, 15)]

olympics$Event <- gsub(", ", " - ", olympics$Event)
olympics$Event <- gsub(",", "", olympics$Event)
olympics$Name <- gsub("\"", "'", olympics$Name)
olympics$Name <- gsub(", ", " ", olympics$Name)
olympics$Team <- gsub("\"", "", olympics$Team)

i = 1
while(i < nrow(olympics))
{
  if(olympics[i, 1] == olympics[i + 1, 1] && olympics[i, 3] == olympics[i + 1, 3])
  {
    olympics$Event[i] <- paste(olympics$Event[i], olympics$Event[i + 1], sep = "$")
    olympics <- olympics[-(i + 1), ]
  }
  else
  {
    i = i + 1;
  }
}

olympics <- olympics[, -1]
write.csv(olympics, "olympicdata.csv")

##### ONCE THE CSV HAS BEEN WRITTEN.... #####
olympics <- read.csv("olympicdata.csv")
olympics <- olympics[,-1]

set.seed(88)
tokeep <- sample(nrow(olympics), 100000, replace = FALSE)


olympics <- olympics[tokeep, ]
row.names(olympics) <- NULL

olympics$Team <- substr(olympics$Team,1,nchar(olympics$Team)-5)

write.csv(olympics, "olympicdata.csv")

pairs <- read.csv("pairs.csv")

pairs$Line_x <- as.integer(pairs$Line_x)
pairs$Line_y <- as.integer(pairs$Line_y)
str(pairs)

head(pairs)
pairs <- pairs[-1,]       

write.csv(pairs, "pairs.csv")
