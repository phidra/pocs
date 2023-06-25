#!/usr/bin/env ruby

require 'csv'

parsings = []

raise "Missing file(s) to parse" unless ARGV.length > 0

ARGV.each {|file|
  puts "About to parse: #{file}"
  parsed = CSV.read(file)
  parsings.push(parsed)
  parsed.each {|line|
    puts "\tNEW LINE :"
    line.each {|token| puts "\t\t '#{token}'" }
  }
  puts "---end---"
}

# si on a passé plusieurs fichiers, indiquer s'ils ont été parsés différemment :
reference = parsings[0]
parsings.each{ |parsing|
  if reference != parsing
    puts "WARNING : input files were parsed differently !"
  end
}
