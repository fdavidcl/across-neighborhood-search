#!/usr/bin/env Rscript

###########################################################################################
## 2010-06-25
## (c) Felix Andrews <felix@nfrac.org>
## GPL-2

## If 'which' is given it should be a logical matrix specifying bold cells.
## Otherwise: in each column or row with numeric data, the maximum or minimum
## value is set bold; 'max' can have entries for each column/row, NA means skip.

## Examples:
## library(xtable)
## x <- tail(iris)
## printbold(xtable(x)) #each = "column"
## printbold(xtable(x), each = "column", max = c(F,NA,NA,T,NA))
## printbold(xtable(x), each = "row", max = FALSE)
## printbold(xtable(x), x >= 6.5, type = "html")

printbold <-
    function(x, which = NULL, each = c("column", "row"), max = TRUE,
             NA.string = "", type = c("latex", "html"),
             sanitize.text.function = force,
             sanitize.rownames.function = NULL,
             sanitize.colnames.function = NULL, ...)
{
    stopifnot(inherits(x, "xtable"))
    each <- match.arg(each)
    type <- match.arg(type)
    digits <- rep(digits(x), length = ncol(x)+1)
    if (!is.null(which)) {
        stopifnot(nrow(which) == nrow(x))
        stopifnot(ncol(which) == ncol(x))
        boldmatrix <- which
    } else {
        boldmatrix <- matrix(FALSE, ncol = ncol(x), nrow = nrow(x))
        ## round values before calculating max/min to avoid trivial diffs
        for (i in 1:ncol(x)) {
            if (!is.numeric(x[,i])) next
            x[,i] <- round(x[,i], digits = digits[i+1])
        }
        if (each == "column") {
            max <- rep(max, length = ncol(x))
            for (i in 1:ncol(x)) {
                xi <- x[,i]
                if (!is.numeric(xi)) next
                if (is.na(max[i])) next
                imax <- max(xi, na.rm = TRUE)
                if (!max[i])
                    imax <- min(xi, na.rm = TRUE)
                boldmatrix[xi == imax, i] <- TRUE
            }
        } else if (each == "row") {
            max <- rep(max, length = nrow(x))
            for (i in 1:nrow(x)) {
                xi <- x[i,]
                ok <- sapply(xi, is.numeric)
                if (!any(ok)) next
                if (is.na(max[i])) next
                imax <- max(unlist(xi[ok]), na.rm = TRUE)
                if (!max[i])
                    imax <- min(unlist(xi[ok]), na.rm = TRUE)
                whichmax <- sapply(xi, identical, imax)
                boldmatrix[i, whichmax] <- TRUE
            }
        }
    }
    ## need to convert to character
    ## only support per-column formats, not cell formats
    display <- rep(display(x), length = ncol(x)+1)
    for (i in 1:ncol(x)) {
        if (!is.numeric(x[,i])) next
        ina <- is.na(x[,i])
        x[,i] <- formatC(x[,i], digits = digits[i+1],
                         format = display[i+1])
        x[ina, i] <- NA.string
        display(x)[i+1] <- "s"
        ## embolden
        yes <- boldmatrix[,i]
        if (type == "latex") {
            x[yes,i] <- paste("\\textbf{", x[yes,i], "}", sep = "")
        } else {
            x[yes,i] <- paste("<strong>", x[yes,i], "</strong>", sep = "")
        }
    }
    print(x, ..., type = type, NA.string = NA.string,
          sanitize.text.function = sanitize.text.function,
          sanitize.rownames.function = sanitize.rownames.function,
          sanitize.colnames.function = sanitize.colnames.function)
}
###########################################################################################

TOP <- 25 # should be 25
basenames <- c("ans_basic", "ans_memory", "hans_simplex", "hans_cmaes", "hans_sw")

d10 <- data.frame()
d30 <- data.frame()

for (basename in basenames) {
  results_basename <- paste0("out/results_", basename)
  diversity_basename <- paste0("out/diversity_", basename)

  results <- data.frame()
  mean_diversity <- list()

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

    while (length(line <- readLines(con, n = 1, warn = FALSE)) > 0) {
      numbers <- strsplit(line, " ")[[1]]
      header <- numbers[1]
      div_vector <- list(as.numeric(numbers[0:-1]))
      mean_diversity[header] <- div_vector
    }

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

  # diversity plot
  for (f in 1:length(mean_diversity)) {
    png(paste0("stats/out/", basename, "_", names(mean_diversity)[f], ".png"))
    plot(mean_diversity[[f]], type = "l", xlab = "cientos de generaciones", ylab = "distancia media entre individuos")
    dev.off()
  }
}

write.csv(d10, file = "out/d10.csv")
write.csv(d30, file = "out/d30.csv")

library(xtable)
sink("doc/d10.tex")
printbold(xtable(
  d10,
  caption = "Medias de ejecuciones con dimensión 10",
  label = "d10"
  ), table.placement = "h!", size = "\\small", sanitize.text.function = function(x) x, sanitize.colnames.function = NULL,
  each = "row", max = FALSE)
sink(NULL)
sink("doc/d30.tex")
printbold(xtable(
  d30,
  caption = "Medias de ejecuciones con dimensión 30",
  label = "d30"
  ), table.placement = "h!", size = "\\small", sanitize.text.function = function(x) x, sanitize.colnames.function = NULL,
  each = "row", max = FALSE)
sink(NULL)


#print(mean_diversity)
