#!/usr/bin/env ruby

ARGV.each do |name|
  (0 ... 25).each do |n|
    `bin/ans #{name} out/results_#{name}#{n}.csv out/diversity_#{name}#{n}.csv`
  end
end
