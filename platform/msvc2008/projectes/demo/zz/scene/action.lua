Action = class("Action")

function Action:initialize()
	self.finish = false
end

function Action:reset()
	self.finish = false
end
