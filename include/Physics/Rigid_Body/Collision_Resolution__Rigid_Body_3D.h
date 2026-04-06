#pragma once

#include <Data_Structures/Map.h>
#include <Stuff/UInt128.h>

#include <Collision_Resolution/Collision_Resolver.h>
#include <Collision_Resolution/Collision_Resolution__Physics_Module__Mesh.h>

#include <Physics/Rigid_Body/Physics_Module__Rigid_Body.h>


namespace LMD
{

    class Collision_Resolution__Rigid_Body_3D : public LPhys::Collision_Resolution
	{
    private:
        LPhys::Collision_Resolution__Physics_Module__Mesh m_default_collision_resolution;

        float m_ws_local_point_distance_tolerance = 0.1f;
        float m_ws_local_point_distance_tolerance_squared = m_ws_local_point_distance_tolerance * m_ws_local_point_distance_tolerance;

        float m_soft_damping_multiplier = 0.75f;

        float m_soft_damping_min_velocity_squared = 0.01f;
        float m_soft_damping_min_angular_velocity_squared = 2.25f;

        float m_hard_damping_velocity_threshold_squared = 0.0001f;
        float m_hard_damping_angular_velocity_threshold_squared = 0.25f;

    private:
        struct WS_Contact_Data
        {
            glm::vec3 local_point_0;
            glm::vec3 local_point_1;
            glm::vec3 normal;

            float accumulated_normal_impulse = 0.0f;
        };
        struct Warm_Starting_Data
        {
            unsigned int frames_since = 0;
            LDS::Vector<WS_Contact_Data> data;
        };

        using Warm_Starting_Data_Map = LDS::Map<LST::UInt128, Warm_Starting_Data>;
        Warm_Starting_Data_Map m_previous_intersections;
        Warm_Starting_Data_Map m_current_intersections;

        struct Prep_Data
        {
            LDS::Vector<WS_Contact_Data*> old_ws_data;
            LDS::Vector<WS_Contact_Data*> new_ws_data;
            LDS::Vector<glm::vec3> radius_vectors_0;
            LDS::Vector<glm::vec3> radius_vectors_1;
        };

    public:
        inline LPhys::Collision_Resolution__Physics_Module__Mesh& default_collision_resolution() { return m_default_collision_resolution; }

        inline void set_soft_damping_multiplier(float _value) { m_soft_damping_multiplier = _value; }

        inline void set_soft_damping_min_velocity(float _value) { m_soft_damping_min_velocity_squared = _value * _value; }
        inline void set_soft_damping_min_angular_velocity(float _value) { m_soft_damping_min_angular_velocity_squared = _value * _value; }

        inline void set_hard_damping_velocity_threshold(float _value) { m_hard_damping_velocity_threshold_squared = _value * _value; }
        inline void set_hard_damping_angular_velocity_threshold(float _value) { m_hard_damping_angular_velocity_threshold_squared = _value * _value; }

    private:
        bool M_resolve_dynamic_vs_dynamic(const LPhys::Intersection_Data& _id, float _dt);
        bool M_resolve_dynamic_vs_static(const LPhys::Intersection_Data& _id, float _dt);

        LST::UInt128 M_construct_pair_key(const LPhys::Physics_Module* _pm_0, const LPhys::Physics_Module* _pm_1) const;
        bool M_ws_data_compatible(const WS_Contact_Data& _data, const glm::vec3& _normal, const glm::vec3& _local_point_0, const glm::vec3& _local_point_1) const;
        glm::vec3 M_calculate_local_contact_point(const LPhys::Physics_Module* _pm, const glm::vec3& _contact_point) const;
        WS_Contact_Data* M_find_ws_data(LDS::Vector<WS_Contact_Data>& _ws_data,
                                           const glm::vec3& _normal,
                                           const glm::vec3& _local_contact_point_0, const glm::vec3& _local_contact_point_1);

        Prep_Data M_calculate_dvs_prep_data(const LPhys::Physics_Module* _dynamic_pm,                   //  dynamic vs static
                                            const LPhys::Physics_Module* _static_pm,
                                            const glm::vec3& _normal,
                                            const LDS::Vector<glm::vec3>& _contact_points);

        void M_apply_old_dvs_data(Physics_Module__Rigid_Body* _dynamic_rb, Prep_Data& _prep_data, const glm::vec3& _normal);

        void M_resolve_dynamic_vs_static_single_point(Physics_Module__Rigid_Body* _rb, const glm::vec3& _radius_vector,
                                                      const glm::vec3& _contact_normal, float _depth, float _dt,
                                                      WS_Contact_Data& _ws_data, bool _apply_restitution);
        void M_apply_impulse(Physics_Module__Rigid_Body* _rb, const glm::vec3& _radius_vector, const glm::vec3& _impulse);
        void M_apply_friction(Physics_Module__Rigid_Body* _rb, const glm::vec3& _radius_vector, const glm::vec3& _contact_normal, float _impulse_magnitude);

        void M_resolve_impulse(Physics_Module__Rigid_Body* _rb, const glm::vec3& _normal, const glm::vec3& _radius_vector, float _normal_velocity, float _mass_denominator);
        void M_resolve_contact(Physics_Module__Rigid_Body* _rb, const glm::vec3& _normal, const glm::vec3& _radius_vector, float _normal_velocity, float _mass_denominator);

        void M_damp_velocities(Physics_Module__Rigid_Body* _rb) const;

	public:
        void on_before_pass() override;
        bool resolve(const LPhys::Intersection_Data &_id, float _dt = 0.0f) override;

	};

}
