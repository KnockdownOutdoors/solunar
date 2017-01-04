s = Gem::Specification.new 'solunar', '0.0.11' do |s|
  s.summary = 'Solunar Forecasting Gem'
  s.description = 'A gem that wraps the C code from Rick Dawson that computes the sun and moon times based on date and coordinates. Takes a series of arguments, and returns a JSON of the hunting/fishing solunar forecast. While designed for hunting and fishing applications, this may be useful to other apps that need solar or lunar data.'
  s.authors = %w["Sullivan Nathan","Dawson Rick"]
  s.extensions = %w[ext/solunar/extconf.rb]
  s.email = "nathan@knockdownoutdoors.com"
  s.homepage = "https://github.com/KnockdownOutdoors/solunar"
  s.license = 'MIT'
  s.files = %w[
    ext/solunar/extconf.rb
    ext/solunar/solunar.c
    ext/solunar/astrocon.h
    lib/solunar.rb
    ext/solunar/DST_Files/USA.txt
    ext/solunar/Data_Files/USA.bin
    ext/solunar/Source_Files/Moon.txt
    ext/solunar/Source_Files/Sun.txt
    ext/solunar/Source_Files/Phase.TXT
    ext/solunar/Source_Files/ilum_2016.txt
    ext/solunar/Source_Files/ilum_2017.txt
    ext/solunar/Source_Files/ilum_2018.txt
    ext/solunar/Source_Files/ilum_2019.txt
    ext/solunar/Source_Files/ilum_2020.txt
    ext/solunar/Source_Files/ilum_2021.txt
    ext/solunar/Source_Files/ilum_2022.txt
    ext/solunar/Source_Files/ilum_2023.txt
    ext/solunar/Source_Files/ilum_2024.txt
    ext/solunar/Source_Files/ilum_2025.txt
    ext/solunar/Source_Files/ilum_2026.txt
    data/DST_Files/USA.txt
    data/Data_Files/USA.bin
    data/Source_Files/Moon.txt
    data/Source_Files/Sun.txt
    data/Source_Files/Phase.TXT
    data/Source_Files/ilum_2016.txt
    data/Source_Files/ilum_2017.txt
    data/Source_Files/ilum_2018.txt
    data/Source_Files/ilum_2019.txt
    data/Source_Files/ilum_2020.txt
    data/Source_Files/ilum_2021.txt
    data/Source_Files/ilum_2022.txt
    data/Source_Files/ilum_2023.txt
    data/Source_Files/ilum_2024.txt
    data/Source_Files/ilum_2025.txt
    data/Source_Files/ilum_2026.txt
  ]
end