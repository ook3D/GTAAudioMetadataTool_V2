#include "MetadataMgr.h"
#include "BaseMetadata.h"
#include "HashManager.h"
#include "BaseMetadata.h"

namespace AMT 
{

    class SpeechMetadataMgr
    {
    public:
        void Write(std::ostream &out);
        void ToJson(ordered_json &j) const;
        void FromJson(const ordered_json &j);
        void Read(std::istream &in);
    };

}; // namespace AMT
