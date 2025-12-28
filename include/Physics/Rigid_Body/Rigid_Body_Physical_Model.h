#pragma once

#include <Physical_Models/Physical_Model.h>

namespace LMD
{

    class Rigid_Body_Physical_Model : public LPhys::Physical_Model
    {
    private:
        float m_mass = 0.0f;
        float m_moment_of_inertia = 0.0f;
        glm::mat3x3 m_inertia_tensor;
        glm::mat3x3 m_inertia_tensor_inverse;

        glm::vec3 m_raw_center_of_mass = {0.0f, 0.0f, 0.0f};
        glm::vec3 m_center_of_mass = {0.0f, 0.0f, 0.0f};

    public:
        Rigid_Body_Physical_Model();
        Rigid_Body_Physical_Model(const Rigid_Body_Physical_Model& _other);
        ~Rigid_Body_Physical_Model();

    private:
        float M_calculate_polygon_area(const LPhys::Polygon& _polygon) const;
        LDS::Vector<float> M_calculate_polygons_areas(const LPhys::Polygon_Holder_Base& _polygons) const;
        float M_calculate_total_area(const LDS::Vector<float>& _polygons_areas) const;

        glm::vec3 M_calculate_central_point() const;
        glm::vec3 M_calculate_center_of_mass(const LPhys::Polygon_Holder_Base& _polygons, const LDS::Vector<float>& _polygons_areas, float _total_area) const;
        float M_calculate_moment_of_inertia(const LPhys::Polygon_Holder_Base& _polygons, const LDS::Vector<float>& _polygons_areas, float _total_area) const;
        glm::mat3x3 M_calculate_inertia_tensor(const LPhys::Polygon_Holder_Base& _polygons, const LDS::Vector<float>& _polygons_areas, float _total_area) const;

    public:
        void update(const glm::mat4x4& _matrix) override;

    public:
        void set_mass(float _mass);
        void recalculate_raw_center_of_mass();

    public:
        inline float mass() const { return m_mass; }
        inline float moment_of_inertia() const { return m_moment_of_inertia; }
        inline const glm::mat3x3& inertia_tensor() const { return m_inertia_tensor; }
        inline const glm::mat3x3& inertia_tensor_inverse() const { return m_inertia_tensor_inverse; }
        inline const glm::vec3& center_of_mass() const { return m_center_of_mass; }
        inline const glm::vec3& raw_center_of_mass() const { return m_raw_center_of_mass; }

    };

}
