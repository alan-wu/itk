# DemonsRegistrationFilter is wrapped with POINTER_WITH_SUPERCLASS
# so this class is not.
WRAP_CLASS("itk::LevelSetMotionRegistrationFilter" POINTER)
  FOREACH(s ${WRAP_ITK_SCALAR})
    WRAP_IMAGE_FILTER_COMBINATIONS("${s}" "${s}" "${WRAP_ITK_VECTOR_REAL}" 2+)
  ENDFOREACH(s)
END_WRAP_CLASS()
