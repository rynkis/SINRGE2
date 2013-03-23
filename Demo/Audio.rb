#encoding:utf-8

module Audio
	
	include Seal
	
	module_function
	
	def init
		Seal.startup("")
		@bgm = nil
		@bgs = nil
		@me = nil
		@se = nil
	end
	
	def update
		
	end
	
	def bgm_play(filename, volume = 100, pitch = 100)
		pitch = 150 if pitch >150
		if not @bgm.nil?
			return bgm_stop if filename.empty?
			if @bgm[1] == filename
				@bgm[0].gain = volume / 100.0
				@bgm[0].pitch = pitch / 100.0
				return
			else
				bgm_stop
			end
		end
		@bgm = [Source.new, filename.clone]
		@bgm[0].looping = true
		@bgm[0].stream = Stream.new(filename)
		@bgm[0].gain = volume / 100.0
		@bgm[0].pitch = pitch / 100.0
		@bgm[0].play
	end
	
	def bgm_stop
		@bgm[0].stop
		@bgm[0].stream.close
		@bgm[0].stream = nil
		@bgm = nil
	end
	
	def bgm_fade(time)
		if not @bgm.nil?
			
		end
	end
	
	def bgs_play(filename, volume = 100, pitch = 100)
		#
	end
	
	def bgs_fade(time)
		#
	end
	
	def me_play(filename, volume = 100, pitch = 100)
		#
	end
	
	def me_fade(time)
		#
	end
	
	def se_play(filename, volume = 100, pitch = 100)
		#
	end
	
	def se_stop
		#
	end
	
	self.init
	
end