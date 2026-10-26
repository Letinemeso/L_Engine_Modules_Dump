#pragma once

#include <L_Debug/L_Debug.h>
#include <Stuff/Coordinates.h>


namespace LMD
{

    template<typename _Stored_Type>
    class Spatial_Grid_3D final
    {
    private:
        _Stored_Type* m_stored_data = nullptr;
        unsigned int m_stored_data_amount = 0;
        LST::Coordinates m_size;

        unsigned int m_layer_size = 0;

    public:
        Spatial_Grid_3D();
        Spatial_Grid_3D(unsigned int _size_x, unsigned int _size_y, unsigned int _size_z);
        Spatial_Grid_3D(const LST::Coordinates& _size);
        Spatial_Grid_3D(const Spatial_Grid_3D& _other);
        Spatial_Grid_3D(Spatial_Grid_3D&& _other);
        void operator=(const Spatial_Grid_3D& _other);
        void operator=(Spatial_Grid_3D&& _other);

        ~Spatial_Grid_3D();

    public:
        inline const LST::Coordinates& size() const { return m_size; }
        inline unsigned int size_x() const { return m_size.x(); }
        inline unsigned int size_y() const { return m_size.y(); }
        inline unsigned int size_z() const { return m_size.z(); }

    public:
        void resize(unsigned int _size_x, unsigned int _size_y, unsigned int _size_z);
        void resize(const LST::Coordinates& _size);
        void clear();
        void fill(const _Stored_Type& _with);

    private:
        unsigned int M_calculate_array_index(const LST::Coordinates& _indices) const;

    public:
        const _Stored_Type& data(unsigned int _x, unsigned int _y, unsigned int _z) const;
        const _Stored_Type& data(const LST::Coordinates& _indices) const;
        _Stored_Type& data(unsigned int _x, unsigned int _y, unsigned int _z);
        _Stored_Type& data(const LST::Coordinates& _indices);

    public:
        bool has_value_inside_area(const LST::Coordinates& _size, const LST::Coordinates& _offset, const _Stored_Type& _value) const;

    };





    template<typename _Stored_Type>
    Spatial_Grid_3D<_Stored_Type>::Spatial_Grid_3D()
    {

    }

    template<typename _Stored_Type>
    Spatial_Grid_3D<_Stored_Type>::Spatial_Grid_3D(unsigned int _size_x, unsigned int _size_y, unsigned int _size_z)
    {
        resize(_size_x, _size_y, _size_z);
    }

    template<typename _Stored_Type>
    Spatial_Grid_3D<_Stored_Type>::Spatial_Grid_3D(const LST::Coordinates& _size)
    {
        resize(_size);
    }

    template<typename _Stored_Type>
    Spatial_Grid_3D<_Stored_Type>::Spatial_Grid_3D(const Spatial_Grid_3D& _other)
    {
        resize(_other.m_size);

        for(unsigned int i = 0; i < m_stored_data_amount; ++i)
            m_stored_data[i] = _other.m_stored_data[i];
    }

    template<typename _Stored_Type>
    Spatial_Grid_3D<_Stored_Type>::Spatial_Grid_3D(Spatial_Grid_3D&& _other)
    {
        m_stored_data = _other.m_stored_data;
        _other.m_stored_data = nullptr;
        m_stored_data_amount = _other.m_stored_data_amount;
        _other.m_stored_data_amount = 0;
        m_size = _other.m_size;
        _other.m_size.x() = 0;
        _other.m_size.y() = 0;
        _other.m_size.z() = 0;
        m_layer_size = _other.m_layer_size;
        _other.m_layer_size = 0;
    }

    template<typename _Stored_Type>
    void Spatial_Grid_3D<_Stored_Type>::operator=(const Spatial_Grid_3D& _other)
    {
        resize(_other.m_size);

        for(unsigned int i = 0; i < m_stored_data_amount; ++i)
            m_stored_data[i] = _other.m_stored_data[i];
    }

    template<typename _Stored_Type>
    void Spatial_Grid_3D<_Stored_Type>::operator=(Spatial_Grid_3D&& _other)
    {
        m_stored_data = _other.m_stored_data;
        _other.m_stored_data = nullptr;
        m_stored_data_amount = _other.m_stored_data_amount;
        _other.m_stored_data_amount = 0;
        m_size = _other.m_size;
        _other.m_size.x() = 0;
        _other.m_size.y() = 0;
        _other.m_size.z() = 0;
        m_layer_size = _other.m_layer_size;
        _other.m_layer_size = 0;
    }


    template<typename _Stored_Type>
    Spatial_Grid_3D<_Stored_Type>::~Spatial_Grid_3D()
    {
        delete[] m_stored_data;
    }



    template<typename _Stored_Type>
    void Spatial_Grid_3D<_Stored_Type>::resize(unsigned int _size_x, unsigned int _size_y, unsigned int _size_z)
    {
        resize({ _size_x, _size_y, _size_z });
    }

    template<typename _Stored_Type>
    void Spatial_Grid_3D<_Stored_Type>::resize(const LST::Coordinates& _size)
    {
        L_ASSERT(_size.x() > 0);
        L_ASSERT(_size.y() > 0);
        L_ASSERT(_size.z() > 0);

        clear();

        m_size = _size;
        m_stored_data_amount = m_size.x() * m_size.y() * m_size.z();

        m_stored_data = new _Stored_Type[m_stored_data_amount];

        m_layer_size = m_size.x() * m_size.z();
    }

    template<typename _Stored_Type>
    void Spatial_Grid_3D<_Stored_Type>::clear()
    {
        delete[] m_stored_data;
        m_stored_data = nullptr;
        m_stored_data_amount = 0;
        m_size = { 0, 0, 0 };
        m_layer_size = 0;
    }

    template<typename _Stored_Type>
    void Spatial_Grid_3D<_Stored_Type>::fill(const _Stored_Type& _with)
    {
        for(unsigned int i = 0; i < m_stored_data_amount; ++i)
            m_stored_data[i] = _with;
    }



    template<typename _Stored_Type>
    unsigned int Spatial_Grid_3D<_Stored_Type>::M_calculate_array_index(const LST::Coordinates& _indices) const
    {
        return (m_layer_size * _indices.y()) + (_indices.x() * m_size.z()) + _indices.z();
    }



    template<typename _Stored_Type>
    const _Stored_Type& Spatial_Grid_3D<_Stored_Type>::data(unsigned int _x, unsigned int _y, unsigned int _z) const
    {
        return data({ _x, _y, _z });
    }

    template<typename _Stored_Type>
    const _Stored_Type& Spatial_Grid_3D<_Stored_Type>::data(const LST::Coordinates& _indices) const
    {
        L_ASSERT(m_stored_data);
        unsigned int array_index = M_calculate_array_index(_indices);
        L_ASSERT(array_index < m_stored_data_amount);
        return m_stored_data[array_index];
    }

    template<typename _Stored_Type>
    _Stored_Type& Spatial_Grid_3D<_Stored_Type>::data(unsigned int _x, unsigned int _y, unsigned int _z)
    {
        return data({ _x, _y, _z });
    }

    template<typename _Stored_Type>
    _Stored_Type& Spatial_Grid_3D<_Stored_Type>::data(const LST::Coordinates& _indices)
    {
        L_ASSERT(m_stored_data);
        unsigned int array_index = M_calculate_array_index(_indices);
        L_ASSERT(array_index < m_stored_data_amount);
        return m_stored_data[array_index];
    }



    template<typename _Stored_Type>
    bool Spatial_Grid_3D<_Stored_Type>::has_value_inside_area(const LST::Coordinates& _size, const LST::Coordinates& _offset, const _Stored_Type& _value) const
    {
        L_ASSERT(_size.x() > 0 && _size.y() > 0 && _size.z() > 0);
        L_ASSERT(_offset.x() + _size.x() <= m_size.x());
        L_ASSERT(_offset.y() + _size.y() <= m_size.y());
        L_ASSERT(_offset.z() + _size.z() <= m_size.z());

        for(unsigned int x = 0; x < _size.x(); ++x)
        {
            for(unsigned int y = 0; y < _size.y(); ++y)
            {
                for(unsigned int z = 0; z < _size.z(); ++z)
                {
                    if(data(_offset.x() + x, _offset.y() + y, _offset.z() + z) == _value)
                        return true;
                }
            }
        }

        return false;
    }

}
