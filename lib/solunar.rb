class Solunar

  NAME = "solunar"
  VERSION = '0.0.10'
  def version
  	"0.0.10"
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
  		day = Hash.new
  		sun = Hash.new
  		moon = Hash.new
  		major_feed_times = Array.new
  		minor_feed_times = Array.new
  		sun[:rise] = segments[1].strip[0..-4] unless segments[1].strip == "NONE"
  		sun[:set] = segments[3].strip[0..-4] unless segments[3].strip == "NONE"
  		sun[:transit] = segments[2].strip[0..-4] unless segments[2].strip == "NONE"
  		moon[:rise] = segments[5].strip[0..-4] unless segments[5].strip == "NONE"
  		moon[:set] = segments[7].strip[0..-4] unless segments[7].strip == "NONE"
  		moon[:transit] = segments[6].strip[0..-4] unless segments[6].strip == "NONE"
  		day[:under_foot] = segments[8].strip[0..-4] unless segments[8].strip == "NONE"
  		unless moon[:rise].nil?
  			minor_feed_times << { start: add_minutes(moon[:rise],-45), stop: add_minutes(moon[:rise],45) }
  		end
  		unless moon[:set].nil?
  			minor_feed_times << { start: add_minutes(moon[:set],-45), stop: add_minutes(moon[:set],45) }
  		end
  		unless moon[:rise].nil? || moon[:set].nil? || moon[:rise].split(":").first.to_i < moon[:set].split(":").first.to_i
  			minor_feed_times.rotate!
  		end
  		unless moon[:transit].nil?
  			major_feed_times << { start: add_minutes(moon[:transit],-90), stop: add_minutes(moon[:transit],90) }
  		end
  		unless day[:under_foot].nil?
  			major_feed_times << { start: add_minutes(day[:under_foot],-90), stop: add_minutes(day[:under_foot],90) }
  		end
  		unless moon[:transit].nil? || day[:under_foot].nil? || moon[:transit].split(":").first.to_i < day[:under_foot].split(":").first.to_i
  			major_feed_times.rotate!
  		end
  		day[:date] = segments[0].strip
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

  def add_hours(str,num)
  	segs = str.split(":")
  	hour = segs[0].to_i
  	hour += num
  	hour += 24 if hour < 0
  	hour -= 24 if hour > 23
  	if hour > 9
  	  return "#{hour.to_s}:#{segs[1]}"
  	else
  	  return "0#{hour.to_s}:#{segs[1]}"
  	end
  end

  def add_minutes(str,num)
    segs = str.split(":")
    hour = segs[0].to_i
    minutes = segs[1].to_i
    minutes += num
    while(minutes > 59)
      minutes -= 60
      hour += 1
    end
    while(minutes < 0)
      minutes += 60
      hour -= 1
    end
    minutes = "0".to_s + minutes.to_s if minutes < 10
    hour += 24 if hour < 0
    hour -= 24 if hour > 23
    if hour > 9
      return "#{hour.to_s}:#{minutes.to_s}"
    else
      return "0#{hour.to_s}:#{minutes.to_s}"
    end
  end

end
require 'solunar/solunar'