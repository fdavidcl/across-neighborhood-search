#!/usr/bin/env Rscript

TOP <- 1 # should be 25
results_basename <- "out/results"
diversity_basename <- "out/diversity"

results <- data.frame()
mean_diversity <- data.frame()

for (i in 1:TOP) {
  results_name <- paste0(results_basename, i, ".csv")
  diversity_name <- paste0(diversity_basename, i, ".log")
  if (!file.exists(results_name))
    system(paste0("bin/ans ", results_name, " ", diversity_name))

  results <- if (length(results) > 0)
      cbind(results, read.csv(results_name))
    else
      read.csv(results_name)

  # Parse diversity file
  con <- file(diversity_name, open = "r")
  dataList <- list()

  while (length(line <- readLines(con, n = 1, warn = FALSE)) > 0) {
    div_vector <- list(as.numeric(strsplit(line, " ")[[1]]))
    dataList <- c(dataList, div_vector)
  }

  mean_diversity <- dataList

  close(con)
}
d10means <- rowMeans(results[c(TRUE, FALSE)])
d30means <- rowMeans(results[c(FALSE, TRUE)])

print(d10means)
print(d30means)
print(mean_diversity)
