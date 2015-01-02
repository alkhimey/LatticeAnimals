require 'uri'
require 'net/http'

DEBUG_MESSAGES = true

class Client
  $server_address = "http://localhost:3000"
  $port = 3000
  def initialize
    @status = :idle
  end
  def get_task
    puts "getting task" if DEBUG_MESSAGES
    res = Net::HTTP.get URI.parse($server_address + "/counter/get_task")
    if res =~ /No task avialable/
      puts "got no task" if DEBUG_MESSAGES
      @status = :wait
    else
      res =~ /<div id=\"task_id\">(.*?)<\/div>.*<div id=\"task_cmd_line\">(.*?)<\/div>/m
      @id, @cmd = $1, $2
      puts "got task no. #{@id}: #{@cmd}"
      @status = :has_task
      @status = :wait if @id == nil
    end
  end
  def do_task
    puts "doing task #{@cmd}" if DEBUG_MESSAGES
    time = Time.now
    @result = `#{@cmd}`
    @time_taken = Time.now - time
    @status = :finished_task
    puts "finished task" if DEBUG_MESSAGES
  end
  def submit_task
    puts "submitting task" if DEBUG_MESSAGES
#     res = Net::HTTP.get_response URI.parse($server_address + "/counter/submit_task")   
#     res.response.body =~ /<input name=\"authenticity_token\" type=\"hidden\" value=\"(.*)\" \/>/
#     token = $1
#     data = {:id => @id, :result => @result, :time => @time_taken, :authenticity_token => token, :commit => "Submit"}.collect{|key, val| "#{key}=#{val}"}.join("&")
#     res = Net::HTTP.post_form URI.parse($server_address + "/counter/submit_task"), {:id => @id, :result => @result, :authenticity_token => token, :commit => "Submit"}
    res = Net::HTTP.post_form URI.parse($server_address + "/counter/submit_task"), {:id => @id, :result => @result, :time => @time_taken}
    @status = :idle
  end
  
  def wait
    puts "sleeping..." if DEBUG_MESSAGES
    sleep 5
    @status = :idle
  end
  
  def run
    while true
      begin
	case @status
	when :idle then get_task
	when :has_task then do_task
	when :finished_task then submit_task
	when :wait then wait
	when :quit then exit
	end
      rescue Errno::ECONNREFUSED, Errno::ECONNRESET
	puts "connection refused, retrying soon..." if DEBUG_MESSAGES
	@status = :wait
	retry
      end
    end
  end
end

Client.new.run