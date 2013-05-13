Actor = class("Actor")

function Actor:initialize(image, x, y, a, sx, sy, w, h)	
	self.hover = false
	self.click = false
	self.image = image
	self.x = x
	self.y = y
	self.angle = a
	self.sx = sx
	self.sy = sy
	self.hWidth = w * 0.5
	self.hHeight = h * 0.5
	
	self.actions = {}
end

function Actor:draw()
	love.graphics.draw(self.image, self.x, self.y, self.angle, self.sx, self.sy, self.hWidth, self.hHeight)
end

function Actor:update(dt)
	for i, v in pairs(self.actions) do
		v:act(dt)
	end
end

function Actor:addAction(action)
	action.actor = self
	table.insert(self.actions, action)
end

function Actor:getPosition()
	return self.x, self.y
end

function Actor:setPosition(x, y)
	self.x = x
	self.y = y
end

function Actor:getRotation()
	return self.angle * 180 / math.pi
end

function Actor:setRotation(degree)
	self.angle = degree * math.pi / 180
end

function Actor:setScale(scale)
	self.sx = scale
	self.sy = scale
end

function Actor:setImage(image)
	self.image = image
end

function Actor:reset()
	for i, v in pairs(self.actions) do
		v:reset()
	end
end

