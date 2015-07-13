#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/RenderPath.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Input/Input.h>

#include "ogtattcam.h"
#include "player.h"

OGTattCam::OGTattCam(Context *context, MasterControl *masterControl):
    Object(context),
    yaw_(0.0f),
    pitch_(88.0f),
    smoothTargetPosition_{Vector3::ZERO},
    smoothTargetVelocity_{Vector3::ZERO}
{
    float viewRange = 50.0f;
    masterControl_ = masterControl;
    SubscribeToEvent(E_UPDATE, HANDLER(OGTattCam, HandleUpdate));

    //Create the camera. Limit far clip distance to match the fog
    rootNode_ = masterControl_->world.scene->CreateChild("CamTrans");
    camera_ = rootNode_->CreateComponent<Camera>();
    camera_->SetFarClip(viewRange);
    camera_->SetNearClip(0.1f);
    camera_->SetFov(30.0f);

    Zone* zone = rootNode_->CreateComponent<Zone>();
    zone->SetBoundingBox(BoundingBox(Vector3(-100.0f, -50.0f, -100.0f), Vector3(100.0f, 50.0f, 100.0f)));
    zone->SetAmbientColor(Color(0.23f, 0.23f, 1.0f, 0.42f));
    zone->SetFogColor(Color(0.42f, 0.3f, 0.23f, 1.0f));
    zone->SetFogStart(10.0f);
    zone->SetFogEnd(viewRange);

    //Set an initial position for the camera scene node above the origin
    rootNode_->SetPosition(Vector3(0.0f, 23.0f, -0.5f));
    rootNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));
    rigidBody_ = rootNode_->CreateComponent<RigidBody>();
    rigidBody_->SetAngularDamping(10.0f);
    rigidBody_->SetLinearDamping(0.9f);
    rigidBody_->SetUseGravity(false);
    CollisionShape* collisionShape = rootNode_->CreateComponent<CollisionShape>();
    collisionShape->SetSphere(0.1f);
    rigidBody_->SetMass(1.0f);

    SetupViewport();
}



void OGTattCam::Start()
{
}

void OGTattCam::Stop()
{
}

void OGTattCam::SetupViewport()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Renderer* renderer = GetSubsystem<Renderer>();

    //Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    SharedPtr<Viewport> viewport(new Viewport(context_, masterControl_->world.scene, camera_));
    viewport_ = viewport;

    //Add anti-asliasing and bloom
    effectRenderPath = viewport_->GetRenderPath()->Clone();
    effectRenderPath->Append(cache->GetResource<XMLFile>("PostProcess/FXAA3.xml"));
    effectRenderPath->SetEnabled("FXAA3", true);
    effectRenderPath->Append(cache->GetResource<XMLFile>("PostProcess/Bloom.xml"));
    effectRenderPath->SetShaderParameter("BloomThreshold", 0.5f);
    effectRenderPath->SetShaderParameter("BloomMix", Vector2(0.75f, 1.0f));
    effectRenderPath->SetEnabled("Bloom", true);

    viewport_->SetRenderPath(effectRenderPath);
    renderer->SetViewport(0, viewport);
}

Vector3 OGTattCam::GetWorldPosition()
{
    return rootNode_->GetWorldPosition();
}

Quaternion OGTattCam::GetRotation()
{
    return rootNode_->GetRotation();
}

void OGTattCam::HandleUpdate(StringHash eventType, VariantMap &eventData)
{
    Vector3 targetPosition = masterControl_->world.player_->GetPosition();
    Vector3 targetVelocity = masterControl_->world.player_->GetLinearVelocity();

    using namespace Update;

    //Take the frame time step, which is stored as a double
    float timeStep = eventData[P_TIMESTEP].GetFloat();
    //Movement speed as world units per second
    const double MOVE_SPEED = 1024.0;
    //Mouse sensitivity as degrees per pixel
    const double MOUSE_SENSITIVITY = 0.1;

    //Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees. Only move the camera when the cursor is hidden.
    Input* input = GetSubsystem<Input>();
    IntVector2 mouseMove = input->GetMouseMove();
    yawDelta_ = 0.5*(yawDelta_ + MOUSE_SENSITIVITY * mouseMove.x_);
    pitchDelta_ = 0.5*(pitchDelta_ + MOUSE_SENSITIVITY * mouseMove.y_);
    yaw_ += rootNode_->GetRotation().y_ + yawDelta_;
    pitch_ += rootNode_->GetRotation().x_ + pitchDelta_;
    pitch_ = Clamp(pitch_, -89.0, 89.0);
    //Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
    //rootNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));

    //Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    Vector3 camForward = rootNode_->GetDirection();
    camForward = Vector3::Scale(camForward, Vector3::ONE - Vector3::UP).Normalized();

    Vector3 camForce = Vector3::ZERO;
    Vector3 centerForce = Vector3::Scale( rootNode_->GetDirection(), Vector3::ONE - Vector3::UP ).Normalized()*0.23f;
    if (input->GetKeyDown('T')) camForce += Vector3::Scale( rootNode_->GetDirection(), Vector3::ONE - Vector3::UP ).Normalized();
    if (input->GetKeyDown('G')) camForce += Vector3::Scale( rootNode_->GetDirection(), -(Vector3::ONE - Vector3::UP) ).Normalized();
    if (input->GetKeyDown('H')) camForce += Vector3::Scale( rootNode_->GetWorldRight(), Vector3::ONE - Vector3::UP ).Normalized() + centerForce;
    if (input->GetKeyDown('F')) camForce += Vector3::Scale( rootNode_->GetWorldRight(), -(Vector3::ONE - Vector3::UP) ).Normalized() + centerForce;
    if (input->GetKeyDown('Y')) camForce += Vector3::UP;
    if (input->GetKeyDown('R') && rootNode_->GetPosition().y_ > 1.0f) camForce += Vector3::DOWN;

    //Read joystick input
    /*JoystickState* joystickState = input->GetJoystickByIndex(0);
    if (joystickState){
        rootNode_->RotateAround(targetPosition, Quaternion(-joystickState->GetAxisPosition(2) * timeStep * 0.1f*MOVE_SPEED, Vector3::UP), TS_WORLD);
        camForce -= joystickState->GetAxisPosition(3) * camForward * timeStep * MOVE_SPEED;
        if (joystickState->GetButtonDown(JB_R2)) camForce += Vector3::UP;
        if (joystickState->GetButtonDown(JB_L2) && rootNode_->GetPosition().y_ > 1.0f) camForce += Vector3::DOWN;

    }*/

    camForce = camForce.Normalized() * MOVE_SPEED;

    if ( forceMultiplier < 8.0 && (input->GetKeyDown(KEY_LSHIFT)||input->GetKeyDown(KEY_RSHIFT)) ){
        forceMultiplier += 0.23;
    } else forceMultiplier = pow(forceMultiplier, 0.75);
    rigidBody_->ApplyForce(forceMultiplier * camForce * timeStep);

    //Prevent camera from going too low
    if (rootNode_->GetPosition().y_ < 1.5f)
    {
        rootNode_->SetPosition(rootNode_->GetPosition().x_, 1.5f, rootNode_->GetPosition().z_);
        rigidBody_->SetLinearVelocity(Vector3(rigidBody_->GetLinearVelocity().x_, 0.0f, rigidBody_->GetLinearVelocity().z_));
    }

    smoothTargetPosition_ = 0.1f * (9.0f * smoothTargetPosition_ + targetPosition);
    smoothTargetVelocity_ = 0.1f * (9.0f * smoothTargetVelocity_ + targetVelocity);
    rootNode_->Translate(smoothTargetVelocity_ * timeStep, TS_WORLD);
    /*
    Quaternion camRot = rootNode_->GetWorldRotation();
    Quaternion aimRotation = camRot;
    aimRotation.FromLookRotation(smoothTargetPosition_ - rootNode_->GetWorldPosition());
    rootNode_->SetRotation(camRot.Slerp(aimRotation, 2.0f*timeStep));
    */

}

void OGTattCam::Lock(SharedPtr<Level> platform)
{
}
