class Solunar

  VERSION = '0.0.1'
  def version
  	"0.0.1"
  end

  def test
  	get_data("2017-05-01",200,29.2108,-81.0228,300,60,0)
  end

  def get_data(date,count,lat,lon,tz_offset_hours,dst_offset_hours,military_time)
  	forecast = Array.new
  	res = generate(date,count,lat,lon,tz_offset_hours,dst_offset_hours,military_time)
  	#Most data comes from the "generate" function, which lives in solunar.c
  	#Major feed times are currently defined as two hours starting at the moon underfoot and moon transit
  	#Minor feed times are one hour after moon rise and moon set
  	res.split(";").each do |line|
  		puts line
  		segments = line.split(",")
  		day = Hash.new
  		sun = Hash.new
  		moon = Hash.new
  		major_feed_times = Array.new
  		minor_feed_times = Array.new
  		sun[:rise] = segments[1].strip unless segments[1].strip == "NONE"
  		sun[:set] = segments[3].strip unless segments[3].strip == "NONE"
  		sun[:transit] = segments[2].strip unless segments[2].strip == "NONE"
  		moon[:rise] = segments[5].strip unless segments[5].strip == "NONE"
  		moon[:set] = segments[7].strip unless segments[7].strip == "NONE"
  		moon[:transit] = segments[6].strip unless segments[6].strip == "NONE"
  		day[:under_foot] = segments[4].strip unless segments[4].strip == "NONE"
  		unless moon[:rise].nil?
  			minor_feed_times << { start: moon[:rise], stop: add_hours(moon[:rise],1) }
  		end
  		unless moon[:set].nil?
  			minor_feed_times << { start: moon[:set], stop: add_hours(moon[:set],1) }
  		end
  		unless moon[:rise].nil? || moon[:set].nil? || moon[:rise].split(":").first.to_i < moon[:set].split(":").first.to_i
  			minor_feed_times.rotate!
  		end
  		unless moon[:transit].nil?
  			major_feed_times << { start: moon[:transit], stop: add_hours(moon[:transit],2) }
  		end
  		unless day[:under_foot].nil?
  			major_feed_times << { start: day[:under_foot], stop: add_hours(day[:under_foot],2) }
  		end
  		unless moon[:transit].nil? || day[:under_foot].nil? || moon[:transit].split(":").first.to_i < day[:under_foot].split(":").first.to_i
  			major_feed_times.rotate!
  		end
  		day[:date] = segments[0].strip
  		day[:days_since_new] = "Unknown"
  		day[:dayscale] = "Unknown"
  		day[:moon_illumination] = segments[12].strip
  		day[:moon_phase] = segments[10].strip
  		day[:moon] = moon
  		day[:sun] = sun
  		day[:major_feed_times] = major_feed_times
  		day[:minor_feed_times] = minor_feed_times
  		forecast<<day
  		#2017-09-08,  8:18 pm,  2:34 am,  8:50 am,  2:33 pm, 10:22 am,  4:39 pm, 11:01 pm,  4:14 am, 240, Waning Gibbous ,         , 93%
  		#s_ris, s_trn, s_set, s_tru, m_ris, m_trn, m_set, m_tru
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
  	  return "#{hour.to_s}:#{segs[1]}:#{segs[2]}"
  	else
  	  return "0#{hour.to_s}:#{segs[1]}:#{segs[2]}"
  	end
  end

end
require 'solunar/solunar'