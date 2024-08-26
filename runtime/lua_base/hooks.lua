hooks	= {}	-- This table holds the hooks that hook into the
				-- c++ engine process thing.

-- This function is called every frame.
-- It is the render function that's called by the C++ engine.
function Engine_Render()
	for k,v in pairs(hooks) do
		-- Lua has no "continue" statement like other languages so this is the best workaround.
		if (type(v) == "function") then goto continue end
		if (v["type"] ~= "Engine_Render") then goto continue end

		v["func"]()

		::continue:: -- this is how a label is defined.
	end
end

function Engine_LoadResources()
	for k,v in pairs(hooks) do
		if (type(v) == "function") then goto continue end
		if (v["type"] ~= "Engine_LoadResources") then goto continue end

		v["func"]()

		::continue::
	end
end

-- This function is used to add a function to the hook table.
function hooks:Add(hookName, hookType, hookFunc)
	hooks[hookName] = {
		["type"] = hookType,
		["func"] = hookFunc
	}
end

function hooks:Remove(hookName)
	print("!!Unimplemented!!")
end