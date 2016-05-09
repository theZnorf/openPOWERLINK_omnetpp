//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

cplusplus {{
    USING_NAMESPACE
}}

namespace oplkMessages;

packet IdentResponsePacket {
    uint8_t flag1;
    uint8_t flag2;
    uint8_t nmtStatus;
    uint8_t identResponseFlags;
    uint8_t powerlinkProfileVersion;
    uint8_t reserved1;
    uint32_t featureFlagsLe;
    uint16_t mtuLe;
    uint16_t pollInSizeLe;
    uint16_t pollOutSizeLe;
    uint32_t responseTimeLe;
    uint16_t reserved2;
    uint32_t deviceTypeLe;
    uint32_t vendorIdLe;
    uint32_t productCodeLe;
    uint32_t revisionNumberLe;
    uint32_t serialNumberLe;
    uint64_t vendorSpecificExt1Le;
    uint32_t verifyConfigurationDateLe;
    uint32_t verifyConfigurationTimeLe;
    uint32_t applicationSwDateLe;
    uint32_t applicationSwTimeLe;
    uint32_t ipAddressLe;
    uint32_t subnetMaskLe;
    uint32_t defaultGatewayLe;
    uint8_t hostName[32];
    uint8_t vendorSpecificExt2[48];
}
