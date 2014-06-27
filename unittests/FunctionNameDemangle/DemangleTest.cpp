#include <stdlib.h>

#include "swift/FunctionNameDemangle/FunctionNameDemangle.h"
#include "gtest/gtest.h"

TEST(FunctionNameDemangleTests, CorrectlyDemangles) {
  char OutputBuffer[128];

  const char *FunctionName = "_TFC3foo3bar3basfS0_FT3zimCS_3zim_T_";
  const char *DemangledName = "foo.bar.bas (foo.bar)(zim : foo.zim) -> ()";

  size_t Result =
      fnd_get_demangled_name(FunctionName, OutputBuffer, sizeof(OutputBuffer));

  EXPECT_STREQ(DemangledName, OutputBuffer);
  EXPECT_EQ(Result, strlen(DemangledName));

  // Make sure the SynthesizeSugarOnTypes option is functioning
  const char *FunctionNameWithSugar = "_TF4main3fooFT3argGSqGSaSi___T_";
  const char *DemangledNameWithSugar = "main.foo (arg : [Swift.Int]?) -> ()";

  Result = fnd_get_demangled_name(FunctionNameWithSugar, OutputBuffer,
                                  sizeof(OutputBuffer));

  EXPECT_STREQ(DemangledNameWithSugar, OutputBuffer);
  EXPECT_EQ(Result, strlen(DemangledNameWithSugar));
}

TEST(FunctionNameDemangledTests, WorksWithNULLBuffer) {
  const char *FunctionName = "_TFC3foo3bar3basfS0_FT3zimCS_3zim_T_";
  const char *DemangledName = "foo.bar.bas (foo.bar)(zim : foo.zim) -> ()";

  // When given a null buffer, fnd_get_demangled_name should still be able to
  // return the size of the demangled string.
  size_t Result = fnd_get_demangled_name(FunctionName, nullptr, 0);

  EXPECT_EQ(Result, strlen(DemangledName));
}

TEST(FunctionNameDemangleTests, IgnoresNonMangledInputs) {
  const char *FunctionName = "printf";
  char OutputBuffer[] = "0123456789abcdef";

  size_t Result =
      fnd_get_demangled_name(FunctionName, OutputBuffer, sizeof(OutputBuffer));

  EXPECT_EQ(0U, Result);
  EXPECT_STREQ("0123456789abcdef", OutputBuffer);
}

