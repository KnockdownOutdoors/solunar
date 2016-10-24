Solunar Calculator Gem
----------------------------

This is the Solunar Forecast calculator developed for Knockdown Outdoors.
It is based on the US Navy data, and calculations developed by Rick
Dawson in the C language. This provides a wrapper around that, for usage
with Ruby applications. It is known to compile on OSX and Linux (tested on Ubuntu).

Installation of this gem requires GCC or an equivalent compiler.

To use the gem, first create an instance of the Solunar class, then
execute the "get_data" method, as follows:

solunar = Solunar.new

solunar.get_data(date,count,lat,lon,tz_offset_hours,dst_offset_hours,12_hour_time)

The arguments are in the following format:

Date - 'YYYY-mm-dd'
This is the start date for the date range to calculate data over.

Count - Integer
This is how many days to calculate data for.

Lat - Float
Latitude of the location to calculate data for.

Lon - Float
Longitude of the location to calculate data for. Use negative for eastern hemisphere.

Tz_offset_hours - Integer
The offset, in hours, of the time zone for the selected location, from UTC. EST(US) is -5.

Dst_offset_hours - Integer
The offset, in hours, of Daylight Savings Time, from normal time, for a location. EST(US) is 1.

12_hour_time - Integer (1/0)
Whether the time should be returned as AM/PM (1) or 24-hour military-style time (0).


The "get_data" method will print the tabular output of the calculations to the screen,
and return a Ruby Hash of the data.

There is a test method, called "test", that will generate a solunar forecast for Daytona Beach, FL,
starting on May 1, 2017, for 200 days.