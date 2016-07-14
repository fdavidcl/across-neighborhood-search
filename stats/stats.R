#!/usr/bin/env Rscript

TOP <- 25 # should be 25
basenames <- c("hans_simplex", "hans_cmaes", "hans_sw")

d10 <- data.frame()
d30 <- data.frame()

for (basename in basenames) {
  results_basename <- paste0("out/results_", basename)
  diversity_basename <- paste0("out/diversity_", basename)

  results <- data.frame()
  mean_diversity <- data.frame()

  for (i in 0:(TOP-1)) {
    results_name <- paste0(results_basename, i, ".csv")
    diversity_name <- paste0(diversity_basename, i, ".csv")
    if (!file.exists(results_name))
      system(paste0("bin/ans ", results_name, " ", diversity_name))

    results <- if (length(results) > 0)
        cbind(results, read.csv(results_name)[1:2])
      else
        read.csv(results_name)[1:2]

    # Parse diversity file
    con <- file(diversity_name, open = "r")
    dataList <- list()

    while (length(line <- readLines(con, n = 1, warn = FALSE)) > 0) {
      numbers <- strsplit(line, " ")[[1]]
      header <- numbers[1]
      div_vector <- list(as.numeric(numbers[0:-1]))
      dataList[header] <- div_vector
    }

    mean_diversity <- dataList

    close(con)
  }
  d10means <- rowMeans(results[c(TRUE, FALSE)])
  d30means <- rowMeans(results[c(FALSE, TRUE)])

  if (length(d10) > 0) {
    d10[basename] = d10means
  } else {
    d10 <- data.frame(d10means)
    names(d10) <- basename
  }

  if (length(d30) > 0) {
    d30[basename] = d30means
  } else {
    d30 <- data.frame(d30means)
    names(d30) <- basename
  }
}

write.csv(d10, file = "out/d10.csv")
write.csv(d30, file = "out/d30.csv")

library(xtable)
sink("latex/d10.tex")
print(xtable(
  d10,
  caption = "Medias de ejecuciones con dimensión 10",
  label = "d10"
  ), table.placement = "h!", size = "\\tiny", sanitize.text.function = function(x) x, sanitize.colnames.function = NULL)
sink(NULL)
sink("latex/d30.tex")
print(xtable(
  d30,
  caption = "Medias de ejecuciones con dimensión 30",
  label = "d30"
  ), table.placement = "h!", size = "\\tiny", sanitize.text.function = function(x) x, sanitize.colnames.function = NULL)
sink(NULL)


#print(mean_diversity)
