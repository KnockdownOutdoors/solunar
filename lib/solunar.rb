class Solunar
  require 'time'

  NAME = "solunar"
  VERSION = '0.0.11'
  def version
  	"0.0.11"
  end

  def test
  	get_data("2017-05-01",100,29.2108,-81.0228,-5,1,0)
  end

  def get_data(date,count,lat,lon,tz_offset_hours,dst_offset_hours,military_time)
  	forecast = Array.new
  	dir = "#{Gem.dir}/gems/#{Solunar::NAME}-#{Solunar::VERSION}/data/"
  	res = generate(date,count,lat,lon,(tz_offset_hours*-60.0).to_i,(dst_offset_hours*60.0).to_i,military_time,dir)
  	#Most data comes from the "generate" function, which lives in solunar.c
  	#Major feed times are currently defined as two hours starting at the moon underfoot and moon transit
  	#Minor feed times are one hour after moon rise and moon set
  	res.split(";").each do |line|
  		next unless line.split(",").length > 7
  		segments = line.split(",")
      iso = "%Y-%m-%dT%H:%M%:z"
      hmt = "%H:%M"
      tz_minutes = segments[9].to_i
      tz_offset = tz_offset_hours >= 0 ? "+" : "-"
      tz_offset = tz_offset + "#{double_digit(tz_minutes/60)}:#{double_digit(tz_minutes%60)}"
      date = segments[0].strip
  		day = Hash.new
  		sun = Hash.new
  		moon = Hash.new
  		major_feed_times = Array.new
  		minor_feed_times = Array.new
  		sun_rise = Time.parse(date+" "+segments[1].strip[0..-4]+tz_offset) unless segments[1].strip == "NONE"
      sun[:rise] = sun_rise.strftime(hmt) unless sun_rise.nil?
      sun[:rise_at] = sun_rise.strftime(iso) unless sun_rise.nil?
  		sun_set = Time.parse(date+" "+segments[3].strip[0..-4]+tz_offset) unless segments[3].strip == "NONE"
      sun[:set] = sun_set.strftime(hmt) unless sun_set.nil?
      sun[:set_at] = sun_set.strftime(iso) unless sun_set.nil?
  		sun_transit = Time.parse(date+" "+segments[2].strip[0..-4]+tz_offset) unless segments[2].strip == "NONE"
      sun[:transit] = sun_transit.strftime(hmt) unless sun_transit.nil?
      sun[:transit_at] = sun_transit.strftime(iso) unless sun_transit.nil?
  		moon_rise = Time.parse(date+" "+segments[5].strip[0..-4]+tz_offset) unless segments[5].strip == "NONE"
      moon[:rise] = moon_rise.strftime(hmt) unless moon_rise.nil?
      moon[:rise_at] = moon_rise.strftime(iso) unless moon_rise.nil?
  		moon_set = Time.parse(date+" "+segments[7].strip[0..-4]+tz_offset) unless segments[7].strip == "NONE"
      moon[:set] = moon_set.strftime(hmt) unless moon_set.nil?
      moon[:set_at] = moon_set.strftime(iso) unless moon_set.nil?
  		moon_transit = Time.parse(date+" "+segments[6].strip[0..-4]+tz_offset) unless segments[6].strip == "NONE"
      moon[:transit] = moon_transit.strftime(hmt) unless moon_transit.nil?
      moon[:transit_at] = moon_transit.strftime(iso) unless moon_transit.nil?
  		day_under_foot = Time.parse(date+" "+segments[8].strip[0..-4]+tz_offset) unless segments[8].strip == "NONE"
      day[:under_foot] = day_under_foot.strftime(hmt) unless day_under_foot.nil?
      day[:under_foot_at] = day_under_foot.strftime(iso) unless day_under_foot.nil?
  		unless moon_rise.nil?
  			minor_feed_times << { 
          start: (moon_rise-45*60).strftime(hmt), 
          start_at: (moon_rise-45*60).strftime(iso),
          stop: (moon_rise+45*60).strftime(hmt),
          stop_at: (moon_rise+45*60).strftime(iso)
        }
  		end
  		unless moon_set.nil?
        minor_feed_times << { 
          start: (moon_set-45*60).strftime(hmt), 
          start_at: (moon_set-45*60).strftime(iso),
          stop: (moon_set+45*60).strftime(hmt),
          stop_at: (moon_set+45*60).strftime(iso)
        }
  		end
  		unless moon_rise.nil? || moon_set.nil? || moon_rise < moon_set
  			minor_feed_times.rotate!
  		end
  		unless moon_transit.nil?
  			major_feed_times << { 
          start: (moon_transit-90*60).strftime(hmt),
          start_at: (moon_transit-90*60).strftime(iso),
          stop: (moon_transit+90*60).strftime(hmt),
          stop_at: (moon_transit+90*60).strftime(iso)
        }
  		end
  		unless day_under_foot.nil?
  			major_feed_times << {
          start: (day_under_foot-90*60).strftime(hmt),
          start_at: (day_under_foot-90*60).strftime(iso),
          stop: (day_under_foot+90*60).strftime(hmt),
          stop_at: (day_under_foot+90*60).strftime(iso)
        }
  		end
  		unless moon_transit.nil? || day_under_foot.nil? || moon_transit < day_under_foot
  			major_feed_times.rotate!
  		end
  		day[:date] = date
      day[:timestamp] = Time.parse(date+" 00:00:00"+tz_offset).strftime(iso)
  		day[:moon_illumination] = segments[12].strip
  		day[:moon_phase] = segments[10].strip
  		day[:moon] = moon
  		day[:sun] = sun
  		day[:major_feed_times] = major_feed_times
  		day[:minor_feed_times] = minor_feed_times
  		forecast<<day
  	end
  	forecast
  end

  def double_digit(num)
    if num > 9 || num < -9
      return num.to_s
    else
      return "0#{num.abs.to_s}"
    end
  end

end

require 'solunar/solunar'