normal_speed = 800;
slow_speed = 700;
threshold = 2705;

local lf_toggle = false;
lf_sensor.set_sensor(lf_toggle);
local last0, last1 = 0, 0;

local left_motor, right_motor = 0, 0;
local left, right = false, false;

while true do
    if bluetooth.available() then
        local bt_thread = coroutine.create(function()
            print(pcall(load(scan())));
        end);
        coroutine.resume(bt_thread);
    end ;

    lf_sensor.set_threshold(threshold);

    if (esp.micros() - last0) > 1 then
        last0 = esp.micros();
        if lf_toggle then
            left = lf_sensor.get_digital();
        else
            right = lf_sensor.get_digital();
        end ;
        lf_toggle = not lf_toggle;
        lf_sensor.set_sensor(lf_toggle);
    end ;

    if (esp.millis() - last1) > 50 then
        last1 = esp.millis();
        us_sensor.trigger();
    end ;

    if (not bluetooth.has_client()) or us_sensor.distance() < 130 or collisionsensor.left() or collisionsensor.right() then
        motor.left(false, 0);
        motor.right(false, 0);
        goto continue;
    end ;

    -- else-if-hell,
    if (left) and (not right) then
        left_motor = normal_speed;
        right_motor = slow_speed;
    elseif (not left) and (right) then
        left_motor = slow_speed;
        right_motor = normal_speed;
    elseif (not left) then
        left_motor = normal_speed;
        right_motor = normal_speed;
    end ;

    motor.left(false, left_motor);
    motor.right(true, right_motor);

    :: continue ::;
end ;
