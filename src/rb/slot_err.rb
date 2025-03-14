# This script is to run when abnormal program termination happens.
# Before building openblink-demo-m5, this file should be byte-compiled by yourself.
# $ mrbc -B slotsleep -o slotsleep.h slotsleep.rb

while true do 
  sleep 5
  puts "press BtnA or update via Blink to escape this loop"
  break if Blink.req_reload? or Input.pressed?
end
