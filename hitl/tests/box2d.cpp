#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include "../PhysicsRobot.h"
#include <box2d/box2d.h>
#include <memory>

TEST_GROUP (PhysicsRobotTestGroup) {
    const float timestep = 1.f / 100;
    const int velocityIterations = 8;
    const int posIterations = 3;
    const float robot_mass = 4.;
    const float pulse_per_mm = 100;

    std::unique_ptr<b2World> world;

    void setup() override
    {
        b2Vec2 gravity(0.0f, 0.0f);
        world = std::make_unique<b2World>(gravity);
    }
};

TEST(PhysicsRobotTestGroup, RobotMovesAccordingToPhysics)
{
    PhysicsRobot robot(*world, 0.3, 0.3, robot_mass, pulse_per_mm, {0., 0.}, 0.);

    for (int i = 0; i < 100; i++) {
        robot.ApplyWheelbaseForces(1., 1.);
        world->Step(timestep, velocityIterations, posIterations);
    }

    auto expected_pos = 0.5 * (2 / robot_mass);
    DOUBLES_EQUAL(expected_pos, robot.GetPosition().x, 0.1);
}

TEST(PhysicsRobotTestGroup, CanReadEncodersWhenMovingStraight)
{
    PhysicsRobot robot(*world, 0.3, 0.3, robot_mass, pulse_per_mm, {0., 0.}, 0.);

    for (int i = 0; i < 100; i++) {
        robot.ApplyWheelbaseForces(1., 1.);
        world->Step(timestep, velocityIterations, posIterations);
        robot.AccumulateWheelEncoders(timestep);
    }

    int left = 0, right = 0;
    int expected = robot.GetPosition().x * 1000 * pulse_per_mm;
    robot.GetWheelEncoders(left, right);
    DOUBLES_EQUAL(expected, left, 1);
    DOUBLES_EQUAL(expected, right, 1);
}

TEST(PhysicsRobotTestGroup, CanReadEncodersWhenTurning)
{
    PhysicsRobot robot(*world, 0.3, 0.3, robot_mass, pulse_per_mm, {0., 0.}, 0.);

    for (int i = 0; i < 100; i++) {
        robot.ApplyWheelbaseForces(-1., 1.);
        world->Step(timestep, velocityIterations, posIterations);
        robot.AccumulateWheelEncoders(timestep);
    }

    int left = 0, right = 0;
    int expected = robot.GetAngle() * 0.5 * 0.3 * pulse_per_mm * 1000;

    robot.GetWheelEncoders(left, right);

    CHECK_EQUAL(expected, right);
    CHECK_EQUAL(-expected, left);
}

class TestRobot : public PhysicsRobot {
public:
    void SetLinearVelocity(b2Vec2 vel)
    {
        robotBody->SetLinearVelocity(vel);
    }

    TestRobot(b2World& world, float size_x, float size_y, float mass, float pulse_per_mm)
        : PhysicsRobot(world, size_x, size_y, mass, pulse_per_mm, {0., 0.}, 0.)
    {
    }
};

TEST(PhysicsRobotTestGroup, DoesNotDriftSideways)
{
    TestRobot robot(*world, 0.3, 0.3, robot_mass, pulse_per_mm);

    // Apply a sideways velocity, tire friction should keep it zero
    robot.SetLinearVelocity({0, 1});

    for (int i = 0; i < 10; i++) {
        robot.ApplyWheelbaseForces(0., 0.);
        world->Step(timestep, velocityIterations, posIterations);
    }

    DOUBLES_EQUAL(0., robot.GetLinearVelocity().y, 0.01);
}
