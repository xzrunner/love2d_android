require("scene.scene")

Scene0 = class("Scene0", Scene)

function Scene0:initialize()
	Scene.initialize(self)
	
	require("scene.scalesize")
	require("scene.changescene")
	require("scene.moveto")
	require("scene.changeimage")
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
	
	self.actors["actor1"] = Actor(graphics["ui_04.png"], 638.0, 322.0, 0.00, 1.0, 1.0, 39.0, 23.0)
	self.actors["actor2"] = Actor(graphics["ui_14.png"], 187.0, 343.0, 0.00, 1.0, 1.0, 74.0, 97.0)
	
	self.buttons["actor0"] = Button(graphics["ui_03.png"], 192.0, 168.0, 0.00, 1.0, 1.0, 39.0, 23.0)
	
	self.buttons.actor0:addPressAction(ScaleSize(1.5))
	self.buttons.actor0:addReleaseAction(ScaleSize(1.0))
	self.buttons.actor0:addReleaseAction(ChangeScene("scene1"))
	self.actors.actor1:addAction(MoveTo(vector(638.0, 322.0), vector(289.0, 129.0), 4.0))
	self.actors.actor1:addAction(ChangeImage(graphics["ui_03.png"]))
	self.actors.actor1:addAction(ScaleSize(2.0))
	self.actors.actor2:addAction(RotateTo(0.0, 343.8, 6.0))
	self.actors.actor2:addAction(MoveTo(vector(187.0, 343.0), vector(300.0, 140.0), 2.3))
end