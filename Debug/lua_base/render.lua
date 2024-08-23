render = { }

function render:Texture()
	new_texture = {
		name	= "UNKNOWN",
		id		= 0
	}

	return new_texture
end

function render:EmptyTexture()
	return self:Texture()
end

function render:OpenTexture(txName)
	new_texture = {
		name = txName,
		id = E_LoadTexture(txName)
	};

	if (id == -1) then
		E_Error("Failed to load texture with name " .. txName)
		return
	end

	return new_texture	
end

function render:UseTexture(tx)
	if (tx.id == -1) then return end

	--print("Bound " .. tx.name)
	E_BindTexture(tx.id)
end

function float_clamp(f)
	if (f > 1.0) then
		f = 1.0
	end

	if (f < 0.0) then
		f = 0.0
	end

	return f
end

function render:SetDrawColour(r, g, b)
	r = float_clamp(r)
	g = float_clamp(g)
	b = float_clamp(b)

	E_SetDrawColour(r, g, b)
end

function render:DrawRect(x, y, w, h)
	E_DrawRect(x, y, w, h)
end