#!/usr/bin/env ruby

ARGV.each do |name|
  pops = name == "l_ans" ? [100, 200] : [nil]
  pops.each do |p|
    fork do
      (0 ... 25).each do |n|
        `bin/ans #{name} out/results_#{p}#{name}#{n}.csv out/diversity_#{p}#{name}#{n}.csv #{p}`
      end
    end
  end
end

Process.waitall
