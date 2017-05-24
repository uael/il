add_subdirectory(cute)
add_subdirectory(mu)

set(jayl_DEPS ${jayl_DEPS} u)
set(jayl_LDADD ${jayl_LDADD} u)

set(jayl_DEPS ${jayl_DEPS} PARENT_SCOPE)
set(jayl_LDADD ${jayl_LDADD} PARENT_SCOPE)
