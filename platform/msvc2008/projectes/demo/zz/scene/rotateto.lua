require("scene.action")

RotateTo = class("RotateTo", Action)

function RotateTo:initialize(beginning, ending, during)
	Action.initialize(self)
	
	self.beginning = beginning
	self.ending = ending
	self.during = during
	self.time = 0
end

function RotateTo:act(dt)
	if self.time < self.during then
		self.time = self.time + dt

		local r = easing.linear(self.time, self.beginning, self.ending - self.beginning, self.during)
		self.actor:setRotation(r)
	else
		self.finish = true
	end
end

function RotateTo:reset()
	Action.reset(self)
	self.time = 0
end