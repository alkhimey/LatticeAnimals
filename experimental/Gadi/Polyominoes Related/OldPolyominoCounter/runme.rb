include Process
sleeptime = 60 * 30 # half an hour
program_ended=false
while (not program_ended)
	fork {load("client.rb")}
	wait
	case $?.exitstatus
		when 0: 	program_ended = true
		when 1:		puts "sleeping"; sleep sleeptime
		when 2: 	File.rename("new_client.rb", "client.rb")
	end
end