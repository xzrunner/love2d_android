require("scene.scene")

Scene1 = class("Scene1", Scene)

function Scene1:initialize()
	Scene.initialize(self)
	
	require("scene.scalesize")
	require("scene.changescene")
	require("scene.moveto")
	require("scene.rotateto")
	
	if not graphics["ui_03.png"] then
		graphics["ui_03.png"] = love.graphics.newImage("ui_03.png")
	end
	if not graphics["ui_04.png"] then
		graphics["ui_04.png"] = love.graphics.newImage("ui_04.png")
	end
	if not graphics["ui_14.png"] then
		graphics["ui_14.png"] = love.graphics.newImage("ui_14.png")
	end
	
	self.buttons["actor3"] = Button(graphics["ui_03.png"], 390.0, 291.0, 0.00, 1.0, 1.0, 39.0, 23.0)
	self.buttons["actor4"] = Button(graphics["ui_04.png"], 85.0, 133.0, 0.00, 1.0, 1.0, 39.0, 23.0)
	self.buttons["actor5"] = Button(graphics["ui_14.png"], 706.0, 366.0, 0.00, 1.0, 1.0, 74.0, 97.0)
	
	self.buttons.actor3:addPressAction(ScaleSize(1.5))
	self.buttons.actor3:addReleaseAction(ChangeScene("scene0"))
	self.buttons.actor3:addReleaseAction(ScaleSize(1.0))
	self.buttons.actor4:addPressAction(MoveTo(vector(85.0, 133.0), vector(622.0, 351.0), 5.8))
	self.buttons.actor5:addPressAction(RotateTo(0.0, 343.8, 4.0))
end
