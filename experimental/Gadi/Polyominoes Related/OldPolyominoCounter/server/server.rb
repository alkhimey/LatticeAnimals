require "socket"
require "monitor"
$MAX_MESSAGE_SIZE = 1000
$PORT=20000
$Version = 1.0

class Server
        def parameters_to_filename(d,n,breakpoint, total_chunks,current_chunk, proper)
                "results/Tree#{proper ? "Proper" : ""}PolyominoCount-#{d}d-#{n}n-breakAt#{breakpoint}-#{current_chunk}of#{total_chunks}.txt"
        end

	def setup_data_array(d,n,breakpoint,total_chunks)
		size=total_chunks.to_i
		temp=[0]*size
		size.times{|current_chunk| temp[current_chunk]=:done if File.file?(parameters_to_filename(d,n,breakpoint,total_chunks,current_chunk,false))}
		return temp
	end

	def get_chunk
		min=@data_array.find_all{|x| Integer===x}.min
		unless min==nil
			temp=@data_array.index(min)
			@data_array[temp]+=1
			return temp+1
		end
		return nil
	end

	def mark_assignment_finished(assignment)
		@data_array[assignment.to_i-1]=:done
	end

	def log(string)
		File.open("log.txt","a"){|file| file.write("#{Time.new}: #{string.chomp}\n")}
	end

	def get_client_first_message(client_sock)
			client_first_message=client_sock.recv($MAX_MESSAGE_SIZE)
			log("Received message from client: #{client_first_message}")
			return client_first_message
	end

	def assign_task_to_client(client_sock)
		log("Assigning task... ")
		current_chunk=0
		@lock.synchronize{current_chunk=get_chunk}
		unless current_chunk==nil
			log("Decided on task #{current_chunk}")
			client_sock.write("#{@d}:#{@n}:#{@breakpoint}:#{current_chunk}:#{@total_chunks}")
		else
			log("Seems like all is finished!")
			client_sock.write("No assignment, but thanks\n")
		end
		client_sock.close
	end

	def receive_results_from_client(client_sock)
		log("Getting results from client... ")
		client_sock.write("Bring it on\n")
		client_answer=client_sock.recv($MAX_MESSAGE_SIZE)
		assignment,filename, client_result=client_answer.split(":::")
		log("Got the following result:\nAssignment=#{assignment}\n#{filename}\n#{client_result}")
		File.open(filename, "w") {|file| file.write(client_result)}
		if filename==parameters_to_filename(@d,@n,@breakpoint,@total_chunks,assignment,false)
			@lock.synchronize{mark_assignment_finished(assignment)}
		else
			log("#{filename} different than #{parameters_to_filename(d,n,breakpoint,total_chunks,assignment)}")
		end
	end

	def report_error_to_client(client_sock)
		log("Not sure what client wants - said \"#{client_first_message}\"")
		client_sock.write("What do you want?\n")
	end

	def handle_client(client_sock)
			client_version, client_first_message=get_client_first_message(client_sock).split(":")
			if $Version > client_version.to_f
				update_client_version(client_sock)
			else
                            if (client_first_message=="receiving new assignment\n")
                                    assign_task_to_client(client_sock)
                            elsif (client_first_message=="sending info\n")
                                    receive_results_from_client(client_sock)
                            else
                                    report_error_to_client(client_sock)
                            end
			end
	end

	def update_client_version(client_sock)
		log("updating client version")
		client_sock.write("new version available")
		if (client_sock.recv(100)=="ready to update")
			log("client is ready, sending updated version")
			new_version=File.open("client.rb", "r"){|file| file.read}
			client_sock.write(new_version.length)
			client_sock.write(new_version)
		else
			report_error_to_client(client_sock)
		end
	end

	def initialize(port)
		if ARGV.size != 4
			puts "usage: server [d] [n] [breakPoint] [totalChunks]"
			exit
		end

		@d=ARGV[0]
		@n=ARGV[1]
		@breakpoint=ARGV[2]
		@total_chunks=ARGV[3]
		@lock=Monitor.new
		@listener=TCPServer.new("", port)
		log("Initialized server")
		@data_array=setup_data_array(@d,@n,@breakpoint,@total_chunks)
		log("Current data array status:\n #{@data_array.inspect}")
	end

	def run
		while true
			Thread.new(@listener.accept) {|client_sock| handle_client(client_sock)}
		end
	end
end #Class Server
Server.new($PORT).run
