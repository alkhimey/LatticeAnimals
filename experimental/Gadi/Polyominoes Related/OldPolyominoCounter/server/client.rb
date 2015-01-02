require "socket"
$IP_ADDRESS="132.68.40.2"
# $IP_ADDRESS="132.68.39.158"
$PORT=20000
$Version = 1.0

def parameters_to_filename(d,n,breakpoint, total_chunks,current_chunk, proper)
	"results/Tree#{proper ? "Proper" : ""}PolyominoCount-#{d}d-#{n}n-breakAt#{breakpoint}-#{current_chunk}of#{total_chunks}.txt"
end
def parameters_to_commandline(d,n,breakpoint, total_chunks,current_chunk)
	"./PolyominoCounter #{d} #{n} #{breakpoint} #{total_chunks} #{current_chunk}"
end

def update_version(socket)
	socket.send("ready to update")
	new_program_size=socket.recv(100).chomp.to_i
	new_program=socket.recv(new_program_size)
	File.open("new_client.rb","w"){|file| file.write(new_program)}
	exit 2 #updated
end

while(true)
	#connecting to receive assignemnt
	begin
		socket=TCPSocket.new($IP_ADDRESS, $PORT)
	rescue Errno::ECONNREFUSED
		puts "Could not connect, server is probably down"
		exit 1 #could not connect
	end
	socket.write("#{$Version}:receiving new assignment\n")
	assignment=socket.recv(100)
	update_version(socket) if (assignemnt == "new version available")
	socket.close
	if (assignment=="No assignment, but thanks\n")
		puts "The computation is complete, thanks for your help!\n"
		exit 0 #regular exit
	end

	#do assignment
	d,n,breakpoint,current_chunk,total_chunks=assignment.split(":")
	commandline=parameters_to_commandline(d,n,breakpoint,total_chunks,current_chunk)
	puts commandline
	`#{commandline}`

	filename=parameters_to_filename(d,n,breakpoint,total_chunks,current_chunk, false)
	results = File.open(filename,"r") { |file| file.read}
	puts results

	filename_proper=parameters_to_filename(d,n,breakpoint,total_chunks,current_chunk, true)
	results_proper = File.open(filename,"r") { |file| file.read}
	puts results

	#connecting to send info
	socket=TCPSocket.new($IP_ADDRESS, $PORT)
	socket.write("sending info\n")
	message=socket.recv(100)
	if (message=="Bring it on\n")
		socket.write("#{current_chunk}:::#{filename}:::#{results}")
		socket.write("#{current_chunk}:::#{filename_proper}:::#{results_proper}")
	end
	socket.close
end