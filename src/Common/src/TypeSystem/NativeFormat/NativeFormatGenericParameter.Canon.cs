﻿// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.


using System;
using Internal.TypeSystem;

using Debug = System.Diagnostics.Debug;

namespace Internal.TypeSystem.NativeFormat
{
    // Implements canonicalization of ECMA generic parameters 
    public sealed partial class NativeFormatGenericParameter : GenericParameterDesc
    {
        public override bool IsCanonicalSubtype(CanonicalFormKind policy)
        {
            Debug.Assert(false, "IsCanonicalSubtype of an indefinite type");
            return false;
        }

        protected override TypeDesc ConvertToCanonFormImpl(CanonicalFormKind kind)
        {
            Debug.Assert(false, "ConvertToCanonFormImpl for an indefinite type");
            return this;
        }
    }
}