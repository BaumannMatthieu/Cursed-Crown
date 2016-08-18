local coroutines = {}

local table_coroutines_wait = {}
local table_coroutines_wait_until = {}

local running_time = 0


thread_manager = {
	-- Method declared in C++ code and is used to 
	-- associate a thread for a script (interactions, attack's script)
	create = function(callback, ...)

		local co = coroutine.create(callback)
		
		local index_co = tostring(co)
		coroutines[index_co] = co
		coroutine.resume(co, ...)

		return index_co
	end,
	
	-- Method declared in C++ code and is called
	-- every game loop. (Re)activate coroutines which
	-- has been suspended during scripts.
	wake_up = function(time, signals) 
		-- we look if coroutines are terminated
		for index,co in pairs(coroutines) do
			local done = coroutine.status(co)

			if done == 'dead' then
				coroutines[index] = nil
			end
		end

		wakeup_wait_threads(time)
		
		--if signals ~= 0 then
		--	wakeup_wait_until_threads(signals)
		--else
		--	print("NO SIGNALS WAKE UP !")
		--end
	end,

	terminated = function(index)
		if coroutines[index] == nil then
			return true
		end
		return false
	end
}

wakeup_wait_threads = function(time)
	running_time = time

	local coroutines_to_wake = {}

	for co, end_time in pairs(table_coroutines_wait) do
		if end_time <= running_time then
			table_coroutines_wait[co] = nil;
			table.insert(coroutines_to_wake, co);
		end
	end

	for _, co in pairs(coroutines_to_wake) do
		coroutine.resume(co)
	end
end

wakeup_wait_until_threads = function(signals)
	local coroutines_to_wake = {}

	for signal in signals do
		if table_coroutines_wait_until[signal] then
			table_coroutines_wait_until[signal] = nil
			
			table.insert(coroutines_to_wake, signal)
		end
	end

	for _, co in pairs(coroutines_to_wake) do
		coroutine.resume(co)
	end
end

-- Yields a thread for a given time specified by pause_time argument
wait = function(pause_time)
	local current_co = coroutine.running()
	
	if current_co ~= nil then
		table_coroutines_wait[current_co] = running_time + pause_time
		coroutine.yield(co)
	else
		print("The main thread cannot be interrupted !")
	end
end

-- Yields a thread until the C++ game send a signal which is
-- specific to each thread.
wait_until = function(callback, ...)
	local current_co = coroutine.running()

	if current_co ~= nil then
		callback(current_co, ...)
		table_coroutines_wait_until[current_co] = true

		coroutine.yield(current_co)
	else 
		print("The main thread cannot be interrupted !")
	end
end
