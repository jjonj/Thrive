namespace thrive {

namespace detail {
template<typename ComponentType>
struct IsRequired {
    
    static const bool value = true;

};


template<typename ComponentType>
struct IsRequired<Optional<ComponentType>> {

    static const bool value = false;

};


template<size_t index, typename... ComponentTypes>
struct ComponentGroupBuilder {

    using ComponentGroup = std::tuple<
        typename ExtractComponentType<ComponentTypes>::PointerType...
    >;

    static bool 
    build(
        EntityManager* entityManager,
        EntityId entityId,
        ComponentGroup& group
    ) {
        using ComponentType = typename std::tuple_element<index, std::tuple<ComponentTypes...>>::type;
        using RawType = typename ExtractComponentType<ComponentType>::Type;
        bool isRequired = IsRequired<ComponentType>::value;
        RawType* component = entityManager->getComponent<RawType>(entityId);
        if (isRequired and not component) {
            return false;
        }
        std::get<index>(group) = component;
        return ComponentGroupBuilder<index-1, ComponentTypes...>::build(entityManager, entityId, group);
    }
        
};


template<typename... ComponentTypes>
struct ComponentGroupBuilder<0, ComponentTypes...> {

    static bool 
    build(
        EntityManager* entityManager,
        EntityId entityId,
        std::tuple<typename ExtractComponentType<ComponentTypes>::PointerType...>& group
    ) {
        using ComponentType = typename std::tuple_element<0, std::tuple<ComponentTypes...>>::type;
        using RawType = typename ExtractComponentType<ComponentType>::Type;
        bool isRequired = IsRequired<ComponentType>::value;
        RawType* component = entityManager->getComponent<RawType>(entityId);
        if (isRequired and not component) {
            return false;
        }
        std::get<0>(group) = component;
        return true;
    }
        
};

} // namespace detail


template<typename... ComponentTypes>
struct EntityFilter<ComponentTypes...>::Implementation {

    Implementation(
        bool recordChanges
    ) : m_recordChanges(recordChanges)
    {
        //Not sure how to handle build() returning false here...? why can it even return false?
        detail::ComponentGroupBuilder<sizeof...(ComponentTypes) - 1, ComponentTypes...>::build(
            m_entityManager,
            id,
            m_group
        );
    }

    void
    initEntities() {
        for (EntityId id : m_entityManager->entities()) {
            this->initEntity(id);
        }
    }

    EntityMap& m_entities;  //Now just a reference
    
    ComponentGroup m_group;
    
    EntityManager* m_entityManager = nullptr;

    bool m_recordChanges;
};

template<typename... ComponentTypes>
EntityFilter<ComponentTypes...>::EntityFilter(
    bool recordChanges
) : m_impl(new Implementation(recordChanges))
{
}


template<typename... ComponentTypes>
typename EntityFilter<ComponentTypes...>::EntityMap::const_iterator
EntityFilter<ComponentTypes...>::begin() const {
    return  m_impl->m_entities.cbegin();
}


template<typename... ComponentTypes>
bool
EntityFilter<ComponentTypes...>::containsEntity(
    EntityId id
) const {
    return  m_impl->m_entities.end();
}


template<typename... ComponentTypes>
typename EntityFilter<ComponentTypes...>::EntityMap::const_iterator
EntityFilter<ComponentTypes...>::end() const {
    return  m_impl->m_entities.cend();
}


template<typename... ComponentTypes>
const typename EntityFilter<ComponentTypes...>::EntityMap&
EntityFilter<ComponentTypes...>::entities() const {
    return m_impl->m_entities;
}


template<typename... ComponentTypes>
void
EntityFilter<ComponentTypes...>::setEntityManager(
    EntityManager* entityManager
) {
    m_impl->m_entities.clear();
    m_impl->m_addedEntities.clear();
    m_impl->m_removedEntities.clear();
    m_impl->m_entityManager = entityManager;
    if (entityManager) {
        m_impl->entityManager->registerComponentGroup(m_group);
        m_entities = m_impl->m_entityManager->getEntitiesFiltered(m_group);
    }
}

} // namespace thrive
